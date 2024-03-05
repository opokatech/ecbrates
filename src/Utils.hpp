#pragma once

#include "Symbol.hpp"

#include <vector>

namespace ECB
{
    class Utils
    {
    public:
        /// Split given string using given delimiter.
        static std::vector<Symbol> split(const std::string &a_string, char a_delimiter);

        /// Makes string uppercase inplace.
        static std::string &uppercase(std::string &a_string);
    };
} // namespace ECB
