#pragma once

#include <string>

// Represents a time point in the format "yyyy-mm-dd".
// No checking is made on the validity of the date.
class Time_Point
{
public:
    Time_Point() = default;

    Time_Point(const std::string &value);

    bool is_set() const { return !m_date.empty(); }

    bool operator==(const Time_Point &other) const { return m_date == other.m_date; }
    bool operator!=(const Time_Point &other) const { return m_date != other.m_date; }
    bool operator<(const Time_Point &other) const { return m_date < other.m_date; }
    bool operator>(const Time_Point &other) const { return m_date > other.m_date; }
    bool operator<=(const Time_Point &other) const { return m_date <= other.m_date; }
    bool operator>=(const Time_Point &other) const { return m_date >= other.m_date; }

    operator std::string() const { return m_date; }

private:
    std::string m_date; // textual representation
};
