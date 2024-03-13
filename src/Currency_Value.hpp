#pragma once

#include "Symbol.hpp"

namespace ECB
{
    struct Currency_Value
    {
        Symbol currency;
        double value;

        bool operator==(const Currency_Value &other) const
        {
            static constexpr double EPSILON = 0.001;
            return currency == other.currency && (std::abs(value - other.value) <= EPSILON);
        }
    };
} // namespace ECB
