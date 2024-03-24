#include "Server_Impl.hpp"
#include "Rates.hpp"
#include "Utils.hpp"

#include "json/json.h"

namespace ECB
{
    Server_Impl::Server_Impl() { mg_mgr_init(&m_mgr); }

    Server_Impl::~Server_Impl() { mg_mgr_free(&m_mgr); }

    bool Server_Impl::is_historical_request(const mg_str *uri) const
    {
        if (uri == nullptr)
            return false;

        static const std::string_view TEMPLATE = "/api/XXXX-XX-XX";

        if (uri->len != TEMPLATE.size())
            return false;

        // uri len is the same as the template
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

    std::string Server_Impl::print_record(const Record &result) const
    {
        Json::Value ans;

        ans["success"] = true;
        ans["base"] = result.Get_Base();
        ans["date"] = static_cast<std::string>(result.Get_Time_Point());
        ans["rates"] = Json::Value();
        auto &ans_rates = ans["rates"]; // alias

        for (const auto &[sym, price]: result.Get_Rates())
            ans_rates[sym] = price;

        std::stringstream ss;

        ss << ans;
        return ss.str();
    }

    std::string Server_Impl::print_error(const std::string &msg) const
    {
        Json::Value ans;

        ans["success"] = false;
        ans["error_text"] = msg;
        std::stringstream ss;

        ss << ans;
        return ss.str();
    }

    void Server_Impl::handle(mg_connection *connection, mg_http_message *message)
    {
        const bool is_latest = mg_http_match_uri(message, "/api/latest");
        const bool is_historical = (!is_latest && is_historical_request(&message->uri));

        if (!is_latest && !is_historical)
        {
            mg_http_reply(connection, 404, "Content-Type: application/json\r\n",
                          print_error("Invalid request").c_str());
            return;
        }

        // this is one of the two possible requests

        // get time point
        const std::optional<Time_Point> tp =
            (is_historical) ? std::make_optional(Time_Point{std::string{message->uri.ptr + 5, message->uri.len - 5}})
                            : m_ecb->Get_Last();

        if (!tp.has_value())
        {
            mg_http_reply(connection, 404, "Content-Type: application/json\r\n", print_error("No record").c_str());
            return;
        }

        // time point is valid, so proceed

        // get base from query
        static const mg_str BASE = mg_str("base");
        std::optional<std::string> base;
        mg_str var_base = mg_http_var(message->query, BASE);
        printf("base: %d, %s\n", (int)var_base.len, var_base.ptr);
        if (var_base.len > 0)
        {
            base = std::string(var_base.ptr, std::max<size_t>(var_base.len, 3u));
            Utils::uppercase(*base);
        }

        const auto record = m_ecb->Get(tp.value(), base);
        if (record)
        {
            const auto json_str = print_record(record.value());
            mg_http_reply(connection, 200, "Content-Type: application/json\r\n", json_str.c_str());
        }
        else
        {
            mg_http_reply(connection, 404, "Content-Type: application/json\r\n",
                          print_error("Record for given time stamp not found").c_str());
        }
    }

    // std::tuple<Symbol, Symbols> Server::get_base_and_symbols(struct mg_connection *a_conn)
    // {
    //     char base_str[100], symbols_str[100];
    //     bool base_set = false, symbols_set = false;

    //     // Get form variables
    //     base_set = (mg_get_var(a_conn, "base", base_str, sizeof(base_str)) > 0);
    //     symbols_set = (mg_get_var(a_conn, "symbols", symbols_str, sizeof(symbols_str)) > 0);

    //     Symbol base = DEFAULT_CURRENCY;
    //     if (base_set)
    //     {
    //         base = base_str;
    //     }

    //     Symbols symbols;
    //     if (symbols_set)
    //     {
    //         symbols = Utils::split(symbols_str, ',');
    //     }

    //     return {base, symbols};
    // }
} // namespace ECB
