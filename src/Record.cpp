#include "Record.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

bool Record::rebase(const std::string &new_base)
{
    if (m_base == new_base)
        return false;

    // find the rate for the new base
    auto new_base_rate = std::find_if(m_rates.begin(), m_rates.end(),
                                      [&new_base](const Currency_Value &cv) { return cv.currency == new_base; });

    if (new_base_rate == m_rates.end())
        return false;

    // save the old value
    double value_of_new_base_in_old_base = new_base_rate->value;

    // just divide all the rates by the value of the new base
    for (auto &cv: m_rates)
        cv.value /= value_of_new_base_in_old_base;

    // replace the rate of the new base with the old base and value
    new_base_rate->currency = m_base;
    new_base_rate->value = 1.0 / value_of_new_base_in_old_base;

    // set the new base
    m_base = new_base;

    return true;
}

std::string Record::as_string(bool with_base) const
{
    std::stringstream ss;

    ss << static_cast<std::string>(m_time_point);

    if (with_base)
        ss << "(" + m_base + ")";

    ss << ":";

    for (const auto &cv: m_rates)
        ss << " " << cv.currency << "(" << std::fixed << std::setprecision(2) << cv.value << ")";
    return ss.str();
}
