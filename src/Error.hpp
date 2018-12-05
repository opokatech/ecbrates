#pragma once

#include <cstdint>
#include <unordered_map>

namespace MO
{
    namespace Error
    {
        using TYPE = uint16_t;

        constexpr TYPE OK             = 200;
        constexpr TYPE INVALID_BASE   = 201;
        constexpr TYPE INVALID_SYMBOL = 202;
        constexpr TYPE NOT_FOUND      = 404;
        constexpr TYPE UNKNOWN_ERROR  = 501;

        static std::unordered_map<TYPE, std::string> TXT = {{OK, "OK"},
                                                            {INVALID_BASE, "Invalid base symbol"},
                                                            {INVALID_SYMBOL, "At least one symbol was invalid"},
                                                            {NOT_FOUND, "Resource not found"},
                                                            {UNKNOWN_ERROR, "Something went wrong"}};
    } // namespace Error
} // namespace MO
