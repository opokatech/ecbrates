#pragma once

#include "Symbol.hpp"

#include "mongoose.h"
// #include "json/forwards.h"

#include <memory>
#include <string>

namespace ECB
{
    class Ecb;

    class Server
    {
    public:
        Server(const Ecb &ecb, uint16_t port);

        ~Server() { destroy(); }

        /// Initialize structures, sets server port.
        bool Initialize();

        /// Stars polling loop.
        void Start();

        /// Polling loop will end after calling stop().
        void Stop() { m_running = false; }

    private:
        /// Clean up the server.
        void destroy();

        /// Checks if request has format /api/yyyy-mm-dd
        static bool is_historical_request(const char *);

        /// Handle requests.
        static int handler(struct mg_connection *a_conn, int a_event, void *a_data);

        /// Handle latest api endpoint.
        static void handle_latest(struct mg_connection *a_conn, Server &a_server);

        /// Handle historical api endpoint.
        static void handle_historical(struct mg_connection *a_conn, Server &a_server);

        /// Outputs result.
        // void print_result(struct mg_connection *a_conn, const Result &);

        const Ecb &m_ecb;
        // std::unique_ptr<Json::StreamWriter> m_json_writer;

        // mongoose server manager
        mg_mgr m_mgr{};
        // mongoose connection
        mg_connection *m_conn = nullptr;

        uint16_t m_port = 0;
        bool m_running = false;
    };
} // namespace ECB
