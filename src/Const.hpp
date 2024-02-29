#pragma once

#include <string>

namespace ECB
{
    namespace URL
    {
        static const std::string ECB_BASE{"https://www.ecb.europa.eu/stats/eurofxref/"};

        static const std::string ECB_HIST = ECB_BASE + "eurofxref-hist.xml";
        static const std::string ECB_DAILY = ECB_BASE + "eurofxref-daily.xml";
    } // namespace URL

} // namespace ECB
