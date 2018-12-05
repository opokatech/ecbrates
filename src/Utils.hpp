#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace MO
{
    namespace Utils
    {
        /// Split given string using given delimiter.
        std::vector<std::string> split(const std::string &a_string, char a_delimiter);

        /// Makes string uppercase in place.
        std::string &uppercase(std::string &a_string);
    } // namespace Utils
} // namespace MO
