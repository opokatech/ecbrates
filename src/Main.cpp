#include <csignal>
// #include <thread>

// #include "Const.hpp"
// #include "Ecb.hpp"
// #include "Exceptions.hpp"
// #include "Server.hpp"

#include "Logging.hpp"
#include "Version_Info.hpp"
#include "options/Converters.hpp"
#include "options/Options.hpp"

#include <iostream>

volatile std::sig_atomic_t keep_running = 1;

void sig_handler(int) { keep_running = 0; }

int main(int argc, char *argv[])
{
    Options::Options options;

    options.add_optional("port", "HTTP port to listen on", "8080", [](const std::string &value) {
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

    // MO::Ecb ecb;

    // if (options.count(MO::OPT_LONG::XML_FILE) > 0)
    // {
    //     auto name = options[MO::OPT_LONG::XML_FILE].as<std::string>();
    //     ecb.Load(MO::Data_Source_File{name.c_str()});

    //     MO::Log("Loaded from file %s (days stored: %ud)\n", name.c_str(), ecb.Get_Timepoint_Count());
    // }

    // if (options.count(MO::OPT_LONG::XML_URL) > 0)
    // {
    //     auto name = options[MO::OPT_LONG::XML_URL].as<std::string>();
    //     ecb.Load(MO::Data_Source_Url{name.c_str()});

    //     MO::Log("Loaded from url %s (days stored: %ud)\n", name.c_str(), ecb.Get_Timepoint_Count());
    // }

    std::signal(SIGINT, sig_handler);

    // // start server if needs to be
    // if (has_port)
    // {
    //     const auto &port_opt = options[MO::OPT_LONG::PORT];
    //     uint16_t port = port_opt.as<uint16_t>();

    //     if (port < MO::OPT::PORT_MIN)
    //     {
    //         throw MO::Exception::Bad_Port(port_opt.as<std::string>());
    //     }

    //     // start server if possible
    //     MO::Server server(ecb, port);

    //     // load data in the same thread
    //     if (ecb.Get_Timepoint_Count() == 0)
    //     {
    //         MO::Log("Loading historical ecb data...");
    //         ecb.Load(MO::Data_Source_Url{MO::URL::ECB_HIST.c_str()});
    //         MO::Log("done (days loaded: %u)\n", ecb.Get_Timepoint_Count());
    //     }

    //     // start thread for serving web requests
    //     std::thread web_thread([&server]() {
    //         MO::Log("starting web server\n");
    //         server.Start();
    //         MO::Log("stopping web server\n");
    //         server.Stop();
    //     });

    //     auto last_time = std::chrono::system_clock::now();
    //     auto update_delay = std::chrono::hours{1};

    //     // this thread will periodically fetch more data
    //     while (true)
    //     {
    //         auto now = std::chrono::system_clock::now();

    //         if (now - last_time > update_delay)
    //         {
    //             last_time = now;
    //             MO::Log("Loading daily ecb data...");
    //             ecb.Load(MO::Data_Source_Url{MO::URL::ECB_DAILY.c_str()});
    //             MO::Log("done (total stored days: %u)\n", ecb.Get_Timepoint_Count());
    //         }

    //         std::this_thread::sleep_for(std::chrono::seconds(1));
    //         if (!keep_running)
    //         {
    //             server.Stop();
    //             MO::Log("Waiting for web thread to join...");
    //             web_thread.join();
    //             MO::Log("done\n");
    //             break;
    //         }
    //     }
    // }

    return 0;
}
