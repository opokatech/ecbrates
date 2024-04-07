#pragma once

#include <memory>

// forward declarations
class Rates;
class Server_Impl;

// Singleton facade class to handle server requests.
// The actual implementation is in Server_Impl. This header file does not depend on mongoose.h.
class Server
{
public:
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;

    static Server &instance();

    ~Server();

    bool initialize(std::shared_ptr<Rates> ecb, uint16_t port, bool listen_all);

    void set_pretty_json(bool pretty);

    void set_precision(uint16_t precision);

    void start_polling();

    void stop_polling();

private:
    Server();

    std::unique_ptr<Server_Impl> m_impl;
};
