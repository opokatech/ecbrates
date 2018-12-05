#include <algorithm>
#include <cstring>
#include <sstream>

#include "mongoose.h"
#include "json/json.h"

#include "Ecb.hpp"
#include "Exceptions.hpp"
#include "Logging.hpp"
#include "Server.hpp"
#include "Utils.hpp"

namespace MO
{
    Server::Server(const Ecb &a_ecb, uint16_t a_port) : m_ecb(a_ecb), m_port(a_port)
    {
        // create json writer
        Json::StreamWriterBuilder sb;

        sb.setDefaults(&sb.settings_);
        sb.settings_["commentStyle"]  = "None";
        sb.settings_["precision"]     = 6;
        sb.settings_["precisionType"] = "decimal";

        m_json_writer = sb.newStreamWriter();

        // create http server
        create();
    }

    // -----------------------------------------------------------------------------------------------------------------

    void Server::Start()
    {
        if (m_server && !m_running)
        {
            MO::Log("Starting on port %s\n", mg_get_option(m_server, "listening_port"));
            m_running = true;
            while (m_running)
            {
                mg_poll_server(m_server, 500);
                // MO::Log("poll...\n");
            }
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    void Server::create()
    {
        if (m_server == nullptr)
        {
            std::stringstream port_string;
            port_string << m_port;

            // Create and configure the server
            m_server        = mg_create_server(this, handler);
            const char *err = mg_set_option(m_server, "listening_port", port_string.str().c_str());

            if (err)
            {
                throw MO::Exception::Bad_Port(err);
            }
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    void Server::destroy()
    {
        if (m_server != nullptr)
        {
            // Cleanup, and free server instance
            mg_destroy_server(&m_server);
            m_server = nullptr;
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    bool Server::is_historical_request(const char *a_uri)
    {
        if (a_uri == nullptr)
            return false;

        std::string_view uri(a_uri);
        static std::string_view TEMPLATE = "/api/XXXX-XX-XX";

        if (uri.size() != TEMPLATE.size())
            return false;

        for (size_t pos = 0; pos < uri.size(); ++pos)
        {
            char c = uri[pos];

            if (TEMPLATE[pos] == 'X') // we expect a digit here
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

    // -----------------------------------------------------------------------------------------------------------------

    std::tuple<Symbol, Symbols> Server::get_base_and_symbols(struct mg_connection *a_conn)
    {
        char base_str[100], symbols_str[100];
        bool base_set = false, symbols_set = false;

        // Get form variables
        base_set    = (mg_get_var(a_conn, "base", base_str, sizeof(base_str)) > 0);
        symbols_set = (mg_get_var(a_conn, "symbols", symbols_str, sizeof(symbols_str)) > 0);

        Symbol base = DEFAULT_CURRENCY;
        if (base_set)
        {
            base = base_str;
        }

        Utils::uppercase(base);

        Symbols symbols;
        if (symbols_set)
        {
            std::transform(symbols_str, symbols_str + strlen(symbols_str), symbols_str,
                           [](char c) { return std::toupper(c); });

            symbols = Utils::split(symbols_str, ',');
        }

        return {base, symbols};
    }

    // -----------------------------------------------------------------------------------------------------------------

    void Server::handle_latest(struct mg_connection *a_conn, Server &a_server)
    {
        auto [base, symbols] = get_base_and_symbols(a_conn);

        const auto &ecb = a_server.Get_Ecb();
        auto result     = ecb.Get_Latest(symbols, base);

        a_server.print_result(a_conn, result);
    }

    // -----------------------------------------------------------------------------------------------------------------

    void Server::handle_historical(struct mg_connection *a_conn, Server &a_server)
    {
        auto [base, symbols] = get_base_and_symbols(a_conn);

        // get uri
        std::string_view uri(a_conn->uri);
        std::string_view uri_timepoint = uri.substr(5); // "/api/yyyy-dd-mm"
        Timepoint tp{std::string{uri_timepoint}};

        const auto &ecb = a_server.Get_Ecb();
        auto result     = ecb.Get_Historical(tp, symbols, base);

        a_server.print_result(a_conn, result);
    }

    // -----------------------------------------------------------------------------------------------------------------

    int Server::handler(struct mg_connection *a_conn, enum mg_event a_event)
    {
        switch (a_event)
        {
        case MG_AUTH:
            return MG_TRUE;
        case MG_REQUEST:
            if (!strcmp(a_conn->uri, "/api/latest"))
            {
                auto srv = reinterpret_cast<Server *>(a_conn->server_param);
                handle_latest(a_conn, *srv);
                return MG_TRUE;
            }
            else if (is_historical_request(a_conn->uri))
            {
                auto srv = reinterpret_cast<Server *>(a_conn->server_param);
                handle_historical(a_conn, *srv);
                return MG_TRUE;
            }
            mg_send_status(a_conn, 404);
            return MG_TRUE;
        default:
            return MG_FALSE;
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    void Server::print_result(struct mg_connection *a_conn, const Result &a_result)
    {
        Json::Value ans;

        if (a_result.historical)
        {
            ans["historical"] = true;
        }

        if (a_result.error != Error::OK)
        {
            ans["success"] = false;
            ans["error"]   = Json::Value();

            ans["error"]["code"] = a_result.error;
            ans["error"]["info"] = Error::TXT[a_result.error];
        }
        else
        {
            ans["success"]  = true;
            ans["base"]     = a_result.base;
            ans["date"]     = a_result.timepoint.get();
            ans["rates"]    = Json::Value();
            auto &ans_rates = ans["rates"]; // alias

            for (const auto &[sym, price] : a_result.prices)
            {
                ans_rates[sym.c_str()] = price;
            }
        }

        std::stringstream ss;

        m_json_writer->write(ans, &ss);
        mg_printf_data(a_conn, ss.str().c_str());
    }
} // namespace MO
