#pragma once

#include <string>

// Represents a currency and its value.
struct Currency_Value
{
    std::string currency;
    double value{};

    bool operator==(const Currency_Value &other) const
    {
        static constexpr double EPSILON = 0.001;
        return currency == other.currency && (std::abs(value - other.value) <= EPSILON);
    }
};
