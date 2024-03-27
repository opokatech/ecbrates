#pragma once

#include "Symbol.hpp"

#include <memory>

namespace ECB
{
    class Rates; // forward declaration
    class Server_Impl;

    // Singleton facade class to handle server requests.
    // The actual implementation is in Server_Impl. This header file does not depend on mongoose.h.
    class Server
    {
    public:
        Server(const Server &) = delete;
        Server &operator=(const Server &) = delete;

        static Server &Instance();

        ~Server();

        bool Initialize(std::shared_ptr<Rates> ecb, uint16_t port, bool listen_all);

        void SetPrettyJson(bool pretty);

        void SetPrecision(uint16_t precision);

        void Start();

        void Stop();

    private:
        Server();

        std::unique_ptr<Server_Impl> m_impl;
    };
} // namespace ECB
