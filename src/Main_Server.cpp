#include "Data_Loader.hpp"
#include "Log.hpp"
#include "Main_Signals.hpp"
#include "Rates.hpp"
#include "Server.hpp"
#include "Urls.hpp"
#include "Version_Info.hpp"

#include "options/Converters.hpp"
#include "options/Parser.hpp"

#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
    Options::Parser arguments;

    // this service allows to
    // - load any data file from file or url - to have something to serve,
    // - serve the data via http server on a given TCP/IP port,
    // - on signal load more data from a url,

    arguments.add_optional("xml_file", "Load data from a file at start", "",
                           [](const std::string &value) { return !value.empty(); });

    arguments.add_optional("xml_url", "Load data from a url at start (used if no file was specified)", ECB_URL_HIST,
                           [](const std::string &value) { return !value.empty(); });

    arguments.add_optional("signal_xml_url", "On signal load more data from this url", ECB_URL_DAILY,
                           [](const std::string &value) { return !value.empty(); });

    arguments.add_mandatory("port", "HTTP port to listen on", [](const std::string &value) {
        uint32_t num = Options::as_uint(value);
        return num >= 1 && num <= 65535; // NOLINT
    });

    arguments.add_optional("precision", "Numerical precision of rates: 1..20", "6", [](const std::string &value) {
        uint32_t num = Options::as_uint(value);
        return num >= 1 && num <= 20; // NOLINT
    });

    arguments.add_optional("log_file", "Log file", "", [](const std::string &param) {
        return !param.empty(); // valid data is not empty
    });

    arguments.add_optional("log_level", "Log level (off, info, warn, error)", "info", [](const std::string &param) {
        return param == "off" || param == "info" || param == "warn" || param == "error";
    });

    arguments.add_flag("listen_all", "Listen on all interfaces");
    arguments.add_flag("pretty", "Pretty print JSON output");
    arguments.add_flag("help", "Show help");

    const bool parse_result = arguments.parse(argc, argv);

    bool show_help = arguments.as_bool("help");

    if (!parse_result || show_help)
    {
        using std::cout, std::endl;

        cout << "ECB Exchange Rates Server" << endl;
        cout << "  Version: " << Version_Info::HASH << endl;
        cout << "  Date   : " << Version_Info::DATE << endl;
        cout << endl;
        cout << "Usage: " << argv[0] << " [options]" << endl;
        cout << arguments.get_possible_options() << endl;

        return -1;
    }

    Log::init(arguments.as_string("log_file"));
    Log::setLevel(arguments.as_string("log_level"));

    auto rates = std::make_shared<Rates>();

    if (!arguments.as_string("xml_file").empty())
    {
        const auto data = Data_Loader::load_from_file(arguments.as_string("xml_file"));

        if (!data.empty())
        {
            rates->add(data);
            Log::info("Loaded {} record(s) from file {}. Total records: {}", data.size(),
                      arguments.as_string("xml_file"), rates->count());
        }
    }
    // if no file was specified, load from the url
    else if (!arguments.as_string("xml_url").empty())
    {
        const auto data = Data_Loader::load_from_url(arguments.as_string("xml_url"));

        if (!data.empty())
        {
            rates->add(data);
            Log::info("Loaded {} record(s) from url {}. Total records: {}", data.size(), arguments.as_string("xml_url"),
                      rates->count());
        }
    }

    Main_Signals::setup(); // handling SIGINT and SIGUSR1

    const uint16_t port = arguments.as_uint("port");
    const uint16_t precision = arguments.as_uint("precision");
    const bool listen_all = arguments.as_bool("listen_all");
    const bool pretty = arguments.as_bool("pretty");

    Log::info("Starting server on port {}{}", port, (listen_all ? " on all interfaces" : " on localhost only"));
    Log::info("Pretty JSON output: {}", (pretty ? "yes" : "no"));
    Log::info("Numerical precision: {}", precision);

    auto &server = Server::instance();
    server.set_pretty_json(pretty);
    server.set_precision(precision);

    if (!server.initialize(rates, port, listen_all))
    {
        Log::error("Failed to initialize server on port {}", port);
        return 1;
    }

    // start thread for serving web requests
    std::thread web_thread([&server]() {
        Log::info("Starting web server in separate thread");
        server.start_polling();
        Log::info("Stopping web server");
        server.stop_polling();
    });

    Log::info("Starting loop in main thread");
    while (Main_Signals::keep_running())
    {
        if (Main_Signals::load_more_data())
        {
            Main_Signals::reset_load_more_data();
            const auto data = Data_Loader::load_from_url(arguments.as_string("signal_xml_url"));
            if (!data.empty())
            {
                rates->add(data);
                Log::info("Loaded {} record(s) from {}, total records: {}", data.size(),
                          arguments.as_string("signal_xml_url"), rates->count());
            }
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }

    Log::info("Stopping the web thread");
    server.stop_polling();

    Log::info("Waiting for web thread to join...");
    web_thread.join();
    Log::info("done");

    return 0;
}
