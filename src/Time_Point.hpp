#pragma once

#include <string>

namespace ECB
{
    /// Represents a time point in the format "yyyy-mm-dd".
    class Time_Point
    {
    public:
        Time_Point() = default;

        Time_Point(const std::string &value);

        bool is_set() const { return !m_date.empty(); }

        operator std::string() const { return m_date; }

    private:
        std::string m_date; // textual representation
    };
} // namespace ECB
