#pragma once

#include <string>

namespace MO
{
    namespace OPT_SHORT
    {
        static const std::string HELP{"h"};
        static const std::string XML_FILE{"f"};
        static const std::string XML_URL{"u"};
        static const std::string PORT{"p"};
    } // namespace OPT_SHORT

    namespace OPT_LONG
    {
        static const std::string HELP{"help"};
        static const std::string XML_FILE{"file"};
        static const std::string XML_URL{"url"};
        static const std::string PORT{"port"};
    } // namespace OPT_LONG

    namespace OPT
    {
        static const std::string HELP     = OPT_SHORT::HELP + "," + OPT_LONG::HELP;
        static const std::string XML_FILE = OPT_SHORT::XML_FILE + "," + OPT_LONG::XML_FILE;
        static const std::string XML_URL  = OPT_SHORT::XML_URL + "," + OPT_LONG::XML_URL;
        static const std::string PORT     = OPT_SHORT::PORT + "," + OPT_LONG::PORT;
        constexpr uint16_t PORT_MIN       = 1;
    } // namespace OPT

    namespace URL
    {
        static const std::string ECB_BASE{"https://www.ecb.europa.eu/stats/eurofxref/"};

        static const std::string ECB_HIST  = ECB_BASE + "eurofxref-hist.xml";
        static const std::string ECB_DAILY = ECB_BASE + "eurofxref-daily.xml";
    } // namespace URL

} // namespace MO
