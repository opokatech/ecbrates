#include "Time_Point.hpp"
#include "Log.hpp"

Time_Point::Time_Point(const std::string &value)
{
    static const std::string TEMPLATE{"dddd-dd-dd"};

    if (value.length() != TEMPLATE.length())
    {
        Log::error("Initializing Time_Point with a string of length {} failed\n", value.length());
        return;
    }

    // check format
    for (size_t pos = 0; pos < TEMPLATE.length(); ++pos)
    {
        const char c = value[pos];

        const bool c_is_expected_digit = (TEMPLATE[pos] == 'd') && (c >= '0' && c <= '9');
        const bool c_is_expected_separator = (TEMPLATE[pos] == '-') && (c == '-');

        if (c_is_expected_digit || c_is_expected_separator)
        {
            continue;
        }
        else
        {
            Log::error("Initializing Time_Point with a string '{}' failed\n", value);
            return;
        }
    }

    m_date = value;
}
