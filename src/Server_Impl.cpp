#include "Server_Impl.hpp"
#include "Log.hpp"
#include "Rates.hpp"
#include "Utils.hpp"

#include "json/json.h"

#include <algorithm>

const std::string_view Server_Impl::HEADER_NORMAL = "Content-Type: application/json\r\n";
const std::string_view Server_Impl::HEADER_ERROR = "Content-Type: application/json\r\n";

Server_Impl::Server_Impl()
{
    mg_mgr_init(&m_mgr);
    m_json_builder["indentation"] = "";
    m_json_builder["precision"] = 6; // default is 17
}

Server_Impl::~Server_Impl() { mg_mgr_free(&m_mgr); }

bool Server_Impl::initialize(std::shared_ptr<Rates> rates, const std::string &listen_address)
{
    if (m_conn != nullptr) // already created
    {
        Log::warn("Server_Impl::initialize() - already initialized");
        return false;
    }

    m_rates = rates;

    m_conn = mg_http_listen(&m_mgr, listen_address.c_str(), handler, this);

    return m_conn != nullptr;
}

bool Server_Impl::is_historical_request(const mg_str *uri) const
{
    if (uri == nullptr)
        return false;

    static const std::string_view TEMPLATE = "/api/XXXX-XX-XX";

    if (uri->len != TEMPLATE.size())
        return false;

    // uri len is the same as the TEMPLATE
    for (size_t pos = 0; pos < uri->len; ++pos)
    {
        const char c = uri->ptr[pos];

        // All "X" must be a digit, all other characters must match
        if (TEMPLATE[pos] == 'X')
        {
            if (c < '0' || c > '9')
                return false;
        }
        else if (c != TEMPLATE[pos])
        {
            return false;
        }
    }
    return true;
}

std::string Server_Impl::print_record(const Record &record, const std::vector<std::string> &symbols) const
{
    Json::Value ans;

    ans["success"] = true;
    ans["base"] = record.base();
    ans["date"] = static_cast<std::string>(record.time_point());
    ans["rates"] = Json::Value(Json::objectValue);
    auto &ans_rates = ans["rates"]; // alias

    if (symbols.empty())
    {
        // return all symbols
        for (const auto &[sym, price]: record.rates())
            ans_rates[sym] = price;
    }
    else
    {
        // filter symbols
        for (const auto &[sym, price]: record.rates())
        {
            if (std::find(symbols.begin(), symbols.end(), sym) != symbols.end())
                ans_rates[sym] = price;
        }
    }

    return Json::writeString(m_json_builder, ans);
}

std::string Server_Impl::print_error(const std::string &msg) const
{
    Json::Value ans;

    ans["success"] = false;
    ans["error_text"] = msg;

    return Json::writeString(m_json_builder, ans);
}

std::optional<std::string> Server_Impl::get_param_base(const mg_http_message *message) const
{
    static const mg_str BASE = mg_str("base");
    mg_str var_base = mg_http_var(message->query, BASE);

    if (var_base.len > 0)
    {
        auto base = std::string(var_base.ptr, std::min<size_t>(var_base.len, 3u));
        return Utils::uppercase(base);
    }
    return {};
}

std::vector<std::string> Server_Impl::get_param_symbols(const mg_http_message *message) const
{
    static const mg_str SYMBOLS = mg_str("symbols");
    static constexpr uint32_t MAX_SYMBOLS_LEN = 4u * 50u; // 4 chars per symbol "eur,"
    mg_str var_symbols = mg_http_var(message->query, SYMBOLS);

    if (var_symbols.len > 0)
    {
        auto symbols_str = std::string(var_symbols.ptr, std::min<size_t>(var_symbols.len, MAX_SYMBOLS_LEN));
        Utils::uppercase(symbols_str);
        return Utils::split(symbols_str, ',');
    }
    return {};
}

void Server_Impl::handler(mg_connection *connection, int event, void *event_data)
{
    if (event == MG_EV_HTTP_MSG)
    {
        auto *hm = reinterpret_cast<mg_http_message *>(event_data);
        auto *impl = reinterpret_cast<Server_Impl *>(connection->fn_data);

        impl->handle(connection, hm);
    }
}

void Server_Impl::handle(mg_connection *connection, mg_http_message *message)
{
    const bool is_latest = mg_http_match_uri(message, "/api/latest");
    const bool is_historical = (!is_latest && is_historical_request(&message->uri));

    if (!is_latest && !is_historical)
    {
        mg_http_reply(connection, 404, HEADER_ERROR.data(), print_error("Invalid request").c_str());
        return;
    }

    // this is one of the two possible requests

    // get time point. In case of historical request, the time point is in the uri /api/XXXX-XX-XX, so
    // we need to extract it skipping the first 5 characters.
    const std::optional<Time_Point> tp =
        (is_historical) ? std::make_optional(Time_Point{std::string{message->uri.ptr + 5, message->uri.len - 5}})
                        : m_rates->last();

    if (!tp.has_value())
    {
        mg_http_reply(connection, 404, HEADER_ERROR.data(), print_error("Time point not found").c_str());
        return;
    }

    // time point is valid, so proceed

    // collect query parameters
    const std::optional<std::string> base = get_param_base(message);
    const std::vector<std::string> symbols = get_param_symbols(message);

    // get record for the given time point
    const auto record = m_rates->get(tp.value(), base);
    if (record.has_value())
    {
        const auto json_str = print_record(record.value(), symbols);
        mg_http_reply(connection, 200, HEADER_NORMAL.data(), json_str.c_str());
    }
    else
    {
        mg_http_reply(connection, 404, HEADER_ERROR.data(),
                      print_error("Record for given time stamp not found").c_str());
    }
}

void Server_Impl::start_polling()
{
    if (m_conn == nullptr || m_running)
    {
        Log::warn("Server_Impl::start_polling() already running");
        return;
    }

    Log::info("Server_Impl::start_polling() starting loop");
    m_running = true;
    while (m_running)
        mg_mgr_poll(&m_mgr, 500);
}
