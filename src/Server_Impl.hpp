#pragma once

#include "mongoose.h"
#include "json/json.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>

class Rates;
class Record;

class Server_Impl
{
public:
    Server_Impl();
    ~Server_Impl();

    bool initialize(std::shared_ptr<Rates> rates, const std::string &listen_address);
    void set_pretty_json(bool pretty) { m_json_builder["indentation"] = pretty ? "  " : ""; }
    void set_precision(uint16_t precision) { m_json_builder["precision"] = precision; }

    void start_polling();
    void stop_polling() { m_running = false; }

private:
    // Handles the request generating a response.
    void handle(mg_connection *connection, mg_http_message *message);

    // Checks if request has format /api/dddd-dd-dd
    bool is_historical_request(const mg_str *) const;

    // Return record as json string.
    std::string print_record(const Record &record, const std::vector<std::string> &symbols) const;

    // Return error message as json string.
    std::string print_error(const std::string &msg) const;

    // Get optionally base if found in the message.
    std::optional<std::string> get_param_base(const mg_http_message *message) const;

    // Get symbols from the message.
    std::vector<std::string> get_param_symbols(const mg_http_message *message) const;

    struct mg_mgr m_mgr = {};
    struct mg_connection *m_conn = nullptr;
    bool m_running = false;
    std::shared_ptr<Rates> m_rates;
    Json::StreamWriterBuilder m_json_builder;

    // must be static to be used as a callback for mongoose
    static void handler(mg_connection *connection, int event, void *event_data);

    const static std::string_view HEADER_NORMAL;
    const static std::string_view HEADER_ERROR;
};
