#pragma once

#include <string>

#include "mongoose.h"

#include "Symbol.hpp"

// ---------------------------------------------------------------------------------------------------------------------

namespace Json
{
    class StreamWriter;
}

namespace MO
{
    class Ecb;
    struct Result;

    class Server {
    public:
        Server(const Ecb &a_ecb, uint16_t a_port);

        ~Server() { destroy(); }

        /// Stars polling loop.
        void Start();

        /// Polling loop will end after calling stop().
        void Stop() { m_running = false; }

        /// Used to get saved
        const Ecb &Get_Ecb() const { return m_ecb; }

    private:
        /// Initialize structures, sets server port.
        void create();

        /// Clean up the server.
        void destroy();

        /// Checks if request has format /api/yyyy-mm-dd
        static bool is_historical_request(const char *);

        /// Handle requests.
        static int handler(struct mg_connection *a_conn, enum mg_event a_event);

        /// Handle latest api endpoint.
        static void handle_latest(struct mg_connection *a_conn, Server &a_server);

        /// Handle historical api endpoint.
        static void handle_historical(struct mg_connection *a_conn, Server &a_server);

        /// Returns base symbol and list of symbols from query string.
        static std::tuple<Symbol, Symbols> get_base_and_symbols(struct mg_connection *a_conn);

        /// Outputs result.
        void print_result(struct mg_connection *a_conn, const Result &);

        const Ecb &m_ecb;
        struct mg_server *m_server        = nullptr;
        uint16_t m_port                   = 0;
        bool m_running                    = false;
        Json::StreamWriter *m_json_writer = nullptr;
    };
} // namespace MO
