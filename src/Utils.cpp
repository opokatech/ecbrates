#include "Utils.hpp"

#include <algorithm>
#include <sstream>

namespace ECB
{
    std::vector<Symbol> Utils::split(const std::string &a_string, char a_delimiter)
    {
        std::stringstream ss(a_string);
        std::string item;
        std::vector<Symbol> elems;

        while (std::getline(ss, item, a_delimiter))
        {
            elems.push_back(item.c_str());
        }
        return elems;
    }

    std::string &Utils::uppercase(std::string &a_string)
    {
        std::transform(a_string.begin(), a_string.end(), a_string.begin(), [](char c) { return std::toupper(c); });

        return a_string;
    }
} // namespace ECB
