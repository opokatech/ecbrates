#pragma once

#include "Currency_Value.hpp"
#include "Time_Point.hpp"

#include <vector>

namespace ECB
{
    // A single "record" consists of a time point, base and a set of values for different currencies.
    // It is a simple container for storing the values.
    class Record
    {
    public:
        Record() = default;

        Record(const Time_Point &tp, const Symbol &base, const std::vector<Currency_Value> &rates)
            : m_time_point(tp), m_base(base), m_rates(rates)
        {
        }

        const Time_Point &Get_Time_Point() const { return m_time_point; }
        const Symbol &Get_Base() const { return m_base; }
        const std::vector<Currency_Value> &Get_Rates() const { return m_rates; }

        // Change the base currency of the record and all the rates accordingly.
        // Returns true if the base was changed, false if it was the same or the new base was not found.
        bool Rebase(const Symbol &new_base);

        std::string Get_As_String_With_Base() const { return get_as_string(true); }
        std::string Get_As_String_Without_Base() const { return get_as_string(false); }

    private:
        std::string get_as_string(bool with_base) const;

        Time_Point m_time_point;
        Symbol m_base;
        std::vector<Currency_Value> m_rates;
    };
} // namespace ECB
