#include "Data_Loader.hpp"
#include "Ecb.hpp"
#include "Logging.hpp"
#include "Main_Signals.hpp"
#include "Server.hpp"
#include "Urls.hpp"
#include "Version_Info.hpp"

#include "options/Converters.hpp"
#include "options/Options.hpp"

#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
    Options::Options options;

    // this service allows to
    // - load any data file from file or url - to have something to serve,
    // - serve the data via http server on a given TCP/IP port,
    // - on signal load more data from the url,
    // - if port is not given then it will serve on the console

    options.add_optional("xml_file", "Load data from a file at start", "",
                         [](const std::string &value) { return !value.empty(); });

    options.add_optional("xml_url", "Load data from a url at start", "",
                         [](const std::string &value) { return !value.empty(); });

    options.add_optional("signal_xml_url", "On signal load more data from this url", "",
                         [](const std::string &value) { return !value.empty(); });

    options.add_optional("port", "HTTP port to listen on", "0", [](const std::string &value) {
        uint32_t num = Options::as_uint(value);
        return num >= 1 && num <= 65535; // NOLINT
    });

    options.add_flag("listen_all", "Listen on all interfaces");

    options.add_flag("help", "Show help");

    const bool parse_result = options.parse(argc, argv);

    bool show_help = options.as_bool("help");

    if (!parse_result || show_help)
    {
        std::cout << "ECB Exchange Rates Server" << std::endl;
        std::cout << "  Version: " << ECB::Version_Info::HASH << std::endl;
        std::cout << "  Date   : " << ECB::Version_Info::DATE << std::endl;
        std::cout << std::endl;
        std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
        std::cout << options.get_possible_options() << std::endl;

        return -1;
    }

    std::cout << "Port: " << options.as_uint("port") << std::endl;

    auto ecb = std::make_shared<ECB::Rates>();

    if (!options.as_string("xml_file").empty())
    {
        const auto data = ECB::Data_Loader::Load_From_File(options.as_string("xml_file"));

        if (!data.empty())
        {
            std::cout << "Loaded " << data.size() << " records from file: " << options.as_string("xml_file")
                      << std::endl;
            ecb->Add(data);
        }
    }

    if (!options.as_string("xml_url").empty())
    {
        const auto data = ECB::Data_Loader::Load_From_Url(options.as_string("xml_url"));

        if (!data.empty())
        {
            std::cout << "Loaded " << data.size() << " records from url: " << options.as_string("xml_url") << std::endl;
            ecb->Add(data);
        }
    }

    Main_Signals::Setup();

    const uint16_t port = options.as_uint("port");
    if (port > 0)
    {
        const bool listen_all = options.as_bool("listen_all");
        std::cout << "Starting server on port " << port << (listen_all ? " on all interfaces" : " on localhost only")
                  << std::endl;

        auto &server = ECB::Server::Instance();

        if (!server.Initialize(ecb, port, listen_all))
        {
            std::cerr << "Failed to initialize server\n";
            return 1;
        }

        // start thread for serving web requests
        std::thread web_thread([&server]() {
            ECB::Log("starting web server\n");
            server.Start();
            ECB::Log("stopping web server\n");
            server.Stop();
        });

        ECB::Log("starting loop until keep_running is non zero\n");
        while (Main_Signals::Keep_Running())
        {
            if (Main_Signals::Load_More_Data())
            {
                Main_Signals::Reset_Load_More_Data();
                std::cout << "load more data" << std::endl;
            }
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
        }

        ECB::Log("stopping the web thread\n");
        server.Stop();

        ECB::Log("Waiting for web thread to join...");
        web_thread.join();
        ECB::Log("done\n");
    }
    else
    {
        std::cout << "Starting console server" << std::endl;
    }

    return 0;
}
