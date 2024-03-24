#pragma once

#include "Symbol.hpp"

#include <memory>
#include <string>

struct mg_connection; // forward declaration

namespace ECB
{
    class Rates; // forward declaration
    class Server_Impl;

    // Singleton class to handle server requests.
    class Server
    {
    public:
        Server(const Server &) = delete;
        Server &operator=(const Server &) = delete;

        static Server& Instance();

        ~Server();

        bool Initialize(std::shared_ptr<Rates> ecb, uint16_t port, bool listen_all);

        /// Stars polling loop.
        void Start();

        /// Polling loop will end after calling stop().
        void Stop();

    private:
        Server();

        std::unique_ptr<Server_Impl> m_impl;

        static void handler(mg_connection *connection, int event, void *event_data);
    };
} // namespace ECB
