#pragma once

#include <string>
#include <vector>

#include "Symbol.hpp"

namespace MO
{
    namespace Utils
    {
        /// Split given string using given delimiter.
        std::vector<Symbol> split(const std::string &a_string, char a_delimiter);

        /// Makes string uppercase inplace.
        std::string &uppercase(std::string &a_string);
    } // namespace Utils
} // namespace MO
