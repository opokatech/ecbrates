#include "Data_Loader.hpp"
#include "Logging.hpp"
#include "Main_Signals.hpp"
#include "Rates.hpp"
#include "Server.hpp"
#include "Urls.hpp"
#include "Version_Info.hpp"

#include "options/Converters.hpp"
#include "options/Options.hpp"

#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
    using std::cout, std::endl;

    Options::Options options;

    // this service allows to
    // - load any data file from file or url - to have something to serve,
    // - serve the data via http server on a given TCP/IP port,
    // - on signal load more data from the url,

    options.add_optional("xml_file", "Load data from a file at start", "",
                         [](const std::string &value) { return !value.empty(); });

    options.add_optional("xml_url", "Load data from a url at start (used if no file was specified)", ECB::ECB_URL_HIST,
                         [](const std::string &value) { return !value.empty(); });

    options.add_optional("signal_xml_url", "On signal load more data from this url", ECB::ECB_URL_DAILY,
                         [](const std::string &value) { return !value.empty(); });

    options.add_mandatory("port", "HTTP port to listen on", [](const std::string &value) {
        uint32_t num = Options::as_uint(value);
        return num >= 1 && num <= 65535; // NOLINT
    });

    options.add_flag("listen_all", "Listen on all interfaces");
    options.add_flag("help", "Show help");

    const bool parse_result = options.parse(argc, argv);

    bool show_help = options.as_bool("help");

    if (!parse_result || show_help)
    {
        cout << "ECB Exchange Rates Server" << endl;
        cout << "  Version: " << ECB::Version_Info::HASH << endl;
        cout << "  Date   : " << ECB::Version_Info::DATE << endl;
        cout << endl;
        cout << "Usage: " << argv[0] << " [options]" << endl;
        cout << options.get_possible_options() << endl;

        return -1;
    }

    auto rates = std::make_shared<ECB::Rates>();

    if (!options.as_string("xml_file").empty())
    {
        const auto data = ECB::Data_Loader::Load_From_File(options.as_string("xml_file"));

        if (!data.empty())
        {
            cout << "Loaded " << data.size() << " records from file: " << options.as_string("xml_file") << endl;
            rates->Add(data);
        }
    }
    // if no file was specified, load from the url
    else if (!options.as_string("xml_url").empty())
    {
        const auto data = ECB::Data_Loader::Load_From_Url(options.as_string("xml_url"));

        if (!data.empty())
        {
            cout << "Loaded " << data.size() << " records from url: " << options.as_string("xml_url") << endl;
            rates->Add(data);
        }
    }

    Main_Signals::Setup(); // handling SIGINT and SIGUSR1

    const uint16_t port = options.as_uint("port");
    const bool listen_all = options.as_bool("listen_all");
    cout << "Starting server on port " << port << (listen_all ? " on all interfaces" : " on localhost only") << endl;

    auto &server = ECB::Server::Instance();

    if (!server.Initialize(rates, port, listen_all))
    {
        std::cerr << "Failed to initialize server\n";
        return 1;
    }

    // start thread for serving web requests
    std::thread web_thread([&server]() {
        ECB::Log("Starting web server in separate thread\n");
        server.Start();
        ECB::Log("Stopping web server\n");
        server.Stop();
    });

    ECB::Log("Starting loop in main thread\n");
    while (Main_Signals::Keep_Running())
    {
        if (Main_Signals::Load_More_Data())
        {
            Main_Signals::Reset_Load_More_Data();
            const auto data = ECB::Data_Loader::Load_From_Url(options.as_string("signal_xml_url"));
            if (!data.empty())
            {
                ECB::Log("Loaded %u records.\n", data.size());
                rates->Add(data);
            }
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }

    ECB::Log("Stopping the web thread\n");
    server.Stop();

    ECB::Log("Waiting for web thread to join...");
    web_thread.join();
    ECB::Log("done\n");

    return 0;
}
