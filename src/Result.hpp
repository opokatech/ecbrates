#pragma once

#include "Error.hpp"
#include "Prices.hpp"
#include "Symbol.hpp"
#include "Timepoint.hpp"

namespace MO
{
    // use it like const auto &[tp, prices, base, ...] = get_rates... ; // structure binding
    struct Result
    {
        Result(bool a_historical = false, Error::TYPE a_error = Error::OK) : historical(a_historical), error(a_error) {}

        Timepoint timepoint;
        Symbol base;
        Prices prices;

        bool historical   = false;
        Error::TYPE error = Error::OK;
    };
} // namespace MO
