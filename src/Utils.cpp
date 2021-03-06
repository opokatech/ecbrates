#include <algorithm>
#include <sstream>
#include <vector>

#include "Utils.hpp"

namespace MO
{
    namespace Utils
    {
        std::vector<Symbol> split(const std::string &a_string, char a_delimiter)
        {
            std::stringstream ss(a_string);
            std::string item;
            Symbols elems;

            while (std::getline(ss, item, a_delimiter))
            {
                elems.push_back(item.c_str());
            }
            return elems;
        }

        // -------------------------------------------------------------------------------------------------------------

        std::string &uppercase(std::string &a_string)
        {
            std::transform(a_string.begin(), a_string.end(), a_string.begin(), [](char c) { return std::toupper(c); });

            return a_string;
        }
    } // namespace Utils
} // namespace MO
