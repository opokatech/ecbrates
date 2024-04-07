#include "Server.hpp"
#include "Server_Impl.hpp"

Server &Server::instance()
{
    static Server instance;
    return instance;
}

Server::~Server() { stop_polling(); }

bool Server::initialize(std::shared_ptr<Rates> ecb, uint16_t port, bool listen_all)
{
    const std::string listening_address =
        std::string("http://") + (listen_all ? "0.0.0.0:" : "127.0.0.1:") + std::to_string(port);

    return m_impl->initialize(ecb, listening_address);
}

void Server::set_pretty_json(bool pretty) { m_impl->set_pretty_json(pretty); }

void Server::set_precision(uint16_t precision) { m_impl->set_precision(precision); }

void Server::start_polling() { m_impl->start_polling(); }

void Server::stop_polling() { m_impl->stop_polling(); }

Server::Server() : m_impl{std::make_unique<Server_Impl>()} {}
