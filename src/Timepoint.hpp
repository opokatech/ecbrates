#pragma once

#include <string>

namespace MO
{
    class Timepoint {
    public:
        Timepoint() = default;

        Timepoint(const std::string &a_date) { set(a_date); }

        /// Throws exception if the parameter can't be used as a timepoint
        void set(const std::string &a_date);

        const std::string &get() const { return m_date; }

        bool is_initialized() const { return m_date.length() > 0; }

        bool operator==(const Timepoint &a_tp) const { return m_date == a_tp.m_date; }
        bool operator!=(const Timepoint &a_tp) const { return !operator==(a_tp); }
        bool operator<(const Timepoint &a_tp) const { return m_date < a_tp.m_date; }

    private:
        std::string m_date; // textual representation

        static const std::string TEMPLATE;
    };
} // namespace MO
