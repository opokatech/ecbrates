#pragma once

#include "Currency_Value.hpp"
#include "Time_Point.hpp"

#include <string>
#include <vector>

// A single "record" consists of a time point, base and a set of values for different currencies.
// It is a simple container for storing the values.
class Record
{
public:
    Record() = default;

    Record(const Time_Point &tp, const std::string &base, const std::vector<Currency_Value> &rates)
        : m_time_point(tp), m_base(base), m_rates(rates)
    {
    }

    const Time_Point &time_point() const { return m_time_point; }
    const std::string &base() const { return m_base; }
    const std::vector<Currency_Value> &rates() const { return m_rates; }

    // Change the base currency of the record and all the rates accordingly.
    // Returns true if the base was changed, false if it was the same or the new base was not found.
    bool rebase(const std::string &new_base);

    std::string as_string_with_base() const { return as_string(true); }
    std::string as_string_without_base() const { return as_string(false); }

private:
    std::string as_string(bool with_base) const;

    Time_Point m_time_point;
    std::string m_base;
    std::vector<Currency_Value> m_rates;
};
