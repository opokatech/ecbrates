#pragma once

#include "mongoose.h"

#include <memory>
#include <string>

namespace ECB
{
    class Rates;
    class Record;

    class Server_Impl
    {
    public:
        Server_Impl();
        ~Server_Impl();

        bool initialize(std::shared_ptr<Rates> rates, const std::string &listen_address);

        void start_polling();
        void stop_polling() { m_running = false; }

    private:
        void handle(mg_connection *connection, mg_http_message *message);
        /// Checks if request has format /api/dddd-dd-dd
        bool is_historical_request(const mg_str *) const;

        std::string print_record(const Record &) const;
        std::string print_error(const std::string &msg) const;

        struct mg_mgr m_mgr = {};
        struct mg_connection *m_conn = nullptr;
        bool m_running = false;
        std::shared_ptr<Rates> m_rates;

        // must be static to be used as a callback for mongoose
        static void handler(mg_connection *connection, int event, void *event_data);
    };
} // namespace ECB
