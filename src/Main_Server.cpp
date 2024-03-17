#include "Data_Loader.hpp"
#include "Ecb.hpp"
#include "Logging.hpp"
#include "Server.hpp"
#include "Urls.hpp"
#include "Version_Info.hpp"
#include "options/Converters.hpp"
#include "options/Options.hpp"

#include <csignal>
#include <iostream>
#include <thread>

volatile std::sig_atomic_t keep_running = 1;
volatile std::sig_atomic_t load_more_data = 0;

void sig_handler_int(int) { keep_running = 0; }
void sig_handler_usr1(int) { load_more_data = 1; }

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

    ECB::Ecb ecb;

    if (!options.as_string("xml_file").empty())
    {
        const auto data = ECB::Data_Loader::Load_From_File(options.as_string("xml_file"));

        if (!data.empty())
        {
            std::cout << "Loaded " << data.size() << " records from file: " << options.as_string("xml_file")
                      << std::endl;
            ecb.Add(data);
        }
        ecb.Add(data);
    }

    if (!options.as_string("xml_url").empty())
    {
        const auto data = ECB::Data_Loader::Load_From_Url(options.as_string("xml_url"));

        if (!data.empty())
        {
            std::cout << "Loaded " << data.size() << " records from url: " << options.as_string("xml_url") << std::endl;
            ecb.Add(data);
        }
    }

    // TODO move somewhere else
    struct sigaction sa_int
    {
    };
    sa_int.sa_handler = sig_handler_int;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    if (sigaction(SIGINT, &sa_int, nullptr) == -1)
    {
        std::cerr << "Failed to set SIGINT handler\n";
        return 1;
    }

    struct sigaction sa_usr1
    {
    };
    sa_usr1.sa_handler = sig_handler_usr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa_usr1, nullptr) == -1)
    {
        std::cerr << "Failed to set SIGUSR1 handler\n";
        return 1;
    }

    const uint16_t port = options.as_uint("port");
    if (port > 0)
    {
        std::cout << "Starting server on port " << options.as_uint("port") << std::endl;
        ECB::Server server(ecb, port);

        if (!server.Initialize())
        {
            std::cerr << "Failed to initialize server\n";
            return 1;
        }

        //     // start thread for serving web requests
        //     std::thread web_thread([&server]() {
        //         ECB::Log("starting web server\n");
        //         server.Start();
        //         ECB::Log("stopping web server\n");
        //         server.Stop();
        //     });

        while (keep_running != 0)
        {
            if (load_more_data != 0)
            {
                load_more_data = 0;
                std::cout << "load more data" << std::endl;
            }
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
        }

        // server.Stop();
        // ECB::Log("Waiting for web thread to join...");
        // web_thread.join();
        // ECB::Log("done\n");
    }
    else
    {
        std::cout << "Starting console server" << std::endl;
    }

    return 0;
}
