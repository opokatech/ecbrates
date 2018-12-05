#include <string>

#include "Exceptions.hpp"
#include "Timepoint.hpp"

namespace MO
{
    const std::string Timepoint::TEMPLATE{"dddd-dd-dd"};

    void Timepoint::set(const std::string &a_date)
    {
        if (a_date.length() < TEMPLATE.length())
        {
            throw Exception::Bad_Timepoint("Date length is wrong");
        }

        // check format
        for (uint8_t pos = 0; pos < TEMPLATE.length(); ++pos)
        {
            const char c = a_date[pos];

            if (TEMPLATE[pos] == 'd')
            {
                if (c < '0' || c > '9')
                {
                    throw Exception::Bad_Timepoint("Expected a digit in date");
                }
            }
            else if (TEMPLATE[pos] != a_date[pos])
            {
                throw Exception::Bad_Timepoint("Given string does not fit into template");
            }
        }

        m_date = a_date;
    }
} // namespace MO
