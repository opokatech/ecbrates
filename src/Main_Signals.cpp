#include "Main_Signals.hpp"

#include <csignal>

volatile bool Main_Signals::m_keep_running = true;    // NOLINT
volatile bool Main_Signals::m_load_more_data = false; // NOLINT

void Main_Signals::setup()
{
    struct sigaction sa_int
    {
    };
    sa_int.sa_handler = sig_handler_int;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    if (sigaction(SIGINT, &sa_int, nullptr) == -1)
    {
        // std::cerr << "Failed to set SIGINT handler\n";
        return;
    }

    struct sigaction sa_usr1
    {
    };
    sa_usr1.sa_handler = sig_handler_usr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa_usr1, nullptr) == -1)
    {
        // std::cerr << "Failed to set SIGUSR1 handler\n";
        return;
    }
}

void Main_Signals::sig_handler_int(int) { m_keep_running = false; }

void Main_Signals::sig_handler_usr1(int) { m_load_more_data = true; }
