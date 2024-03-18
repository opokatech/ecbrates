#pragma once

class Main_Signals
{
public:
    Main_Signals() = delete;
    Main_Signals(const Main_Signals &) = delete;
    Main_Signals &operator=(const Main_Signals &) = delete;

    static void Setup();
    static bool Keep_Running() { return m_keep_running; }
    static bool Load_More_Data() { return m_load_more_data; }
    static void Reset_Load_More_Data() { m_load_more_data = false; }

private:
    static void sig_handler_int(int);
    static void sig_handler_usr1(int);

    static volatile bool m_keep_running;
    static volatile bool m_load_more_data;
};
