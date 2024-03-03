#pragma once

#include "Currency_Value.hpp"
#include "Symbol.hpp"
#include "Time_Point.hpp"

#include <vector>

namespace ECB
{
    // A single "record" consists of a time point, base and a set of values for different currencies.
    // It is a simple container for storing the values.
    class Record
    {
    public:
        Record(const Time_Point &tp, const Symbol &base, const std::vector<Currency_Value> &rates)
            : m_time_point(tp), m_base(base), m_rates(rates)
        {
        }

        const Time_Point &Get_Time_Point() const { return m_time_point; }
        const Symbol &Get_Base() const { return m_base; }
        const std::vector<Currency_Value> &Get_Rates() const { return m_rates; }

    private:
        Time_Point m_time_point;
        Symbol m_base;
        std::vector<Currency_Value> m_rates;
    };
} // namespace ECB
