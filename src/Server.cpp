#include "Server.hpp"
#include "Ecb.hpp"
#include "Logging.hpp"
#include "Server_Impl.hpp"
#include "Utils.hpp"

#include "mongoose.h"
#include "json/json.h"

#include <cstring>
#include <sstream>

namespace ECB
{
    std::shared_ptr<Server> Server::Instance()
    {
        static std::shared_ptr<Server> instance(new Server());

        return instance;
    }

    Server::~Server()
    {
        // TODO
    }

    bool Server::Initialize(std::shared_ptr<Ecb> ecb, uint16_t port, bool listen_all)
    {
        if (m_impl->m_conn != nullptr)
            return false;

        m_impl->m_ecb = ecb;
        m_impl->m_port = port;

        const std::string listening_address =
            std::string("http://") + (listen_all ? "0.0.0.0:" : "127.0.0.1:") + std::to_string(port);

        m_impl->m_conn = mg_http_listen(&m_impl->m_mgr, listening_address.c_str(), handler, m_impl.get());

        return m_impl->m_conn != nullptr;
    }

    void Server::Start()
    {
        if (m_impl->m_conn != nullptr && !m_impl->m_running)
        {
            ECB::Log("Starting on port %d\n", m_impl->m_port);
            m_impl->m_running = true;
            while (m_impl->m_running)
            {
                mg_mgr_poll(&m_impl->m_mgr, 500);
            }
        }
    }

    void Server::Stop() { m_impl->m_running = false; }

    Server::Server() : m_impl{std::make_unique<Server_Impl>()} {}

    void Server::handler(mg_connection *connection, int event, void *event_data)
    {
        if (event == MG_EV_HTTP_MSG)
        {
            mg_http_message *hm = reinterpret_cast<mg_http_message *>(event_data);
            Server_Impl *impl = reinterpret_cast<Server_Impl *>(connection->fn_data);

            impl->handle(connection, hm);
        }
    }
} // namespace ECB
