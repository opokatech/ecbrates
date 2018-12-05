#include <csignal>
#include <thread>

#include "cxxopts.hpp"

#include "Const.hpp"
#include "Ecb.hpp"
#include "Exceptions.hpp"
#include "Logging.hpp"
#include "Server.hpp"

volatile std::sig_atomic_t keep_running = 1;

void sig_handler(int) { keep_running = 0; }

int main(int argc, char *argv[])
{
    cxxopts::Options opts("ecbrates", "Daemon for serving currency rates, version: TODO");

    opts.add_options()(MO::OPT::HELP, "Show help")

        // initialize with a file
        (MO::OPT::XML_FILE, "XML file with data", cxxopts::value<std::string>())
        // initialize from url
        (MO::OPT::XML_URL, "URL for fetching current data in XML format", cxxopts::value<std::string>())

            (MO::OPT::PORT, "HTTP port to listen on", cxxopts::value<uint16_t>())
        // keep updating latest data
        ;

    auto options   = opts.parse(argc, argv);
    bool has_port  = options.count(MO::OPT_LONG::PORT);
    bool show_help = options.count(MO::OPT_LONG::HELP);

    if (show_help || !has_port)
    {
        std::cout << opts.help() << std::endl;
        return 0;
    }

    MO::Ecb ecb;

    if (options.count(MO::OPT_LONG::XML_FILE) > 0)
    {
        auto name = options[MO::OPT_LONG::XML_FILE].as<std::string>();
        ecb.Load(MO::Data_Source_File{name.c_str()});

        MO::Log("Loaded from file %s (days stored: %ud)\n", name.c_str(), ecb.Get_Timepoint_Count());
    }

    if (options.count(MO::OPT_LONG::XML_URL) > 0)
    {
        auto name = options[MO::OPT_LONG::XML_URL].as<std::string>();
        ecb.Load(MO::Data_Source_Url{name.c_str()});

        MO::Log("Loaded from url %s (days stored: %ud)\n", name.c_str(), ecb.Get_Timepoint_Count());
    }

    std::signal(SIGINT, sig_handler);

    // start server if needs to be
    if (has_port)
    {
        const auto &port_opt = options[MO::OPT_LONG::PORT];
        uint16_t port        = port_opt.as<uint16_t>();

        if (port < MO::OPT::PORT_MIN)
        {
            throw MO::Exception::Bad_Port(port_opt.as<std::string>());
        }

        // start server if possible
        MO::Server server(ecb, port);

        // load data in the same thread
        if (ecb.Get_Timepoint_Count() == 0)
        {
            MO::Log("Loading historical ecb data...");
            ecb.Load(MO::Data_Source_Url{MO::URL::ECB_HIST.c_str()});
            MO::Log("done (days loaded: %u)\n", ecb.Get_Timepoint_Count());
        }

        // start thread for serving web requests
        std::thread web_thread([&server]() {
            MO::Log("starting web server\n");
            server.Start();
            MO::Log("stopping web server\n");
            server.Stop();
        });

        auto last_time    = std::chrono::system_clock::now();
        auto update_delay = std::chrono::hours{1};

        // this thread will periodically fetch more data
        while (true)
        {
            auto now = std::chrono::system_clock::now();

            if (now - last_time > update_delay)
            {
                last_time = now;
                MO::Log("Loading daily ecb data...");
                ecb.Load(MO::Data_Source_Url{MO::URL::ECB_DAILY.c_str()});
                MO::Log("done (total stored days: %u)\n", ecb.Get_Timepoint_Count());
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (!keep_running)
            {
                server.Stop();
                MO::Log("Waiting for web thread to join...");
                web_thread.join();
                MO::Log("done\n");
                break;
            }
        }
    }

    return 0;
}
