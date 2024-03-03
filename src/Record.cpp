#include "Record.hpp"

#include <iomanip>
#include <sstream>

namespace ECB
{
    std::string Record::get_as_string(bool with_base) const
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
} // namespace ECB
