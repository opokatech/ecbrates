#pragma once

#include <stdexcept>

namespace MO
{
    namespace Exception
    {
        struct Bad_Timepoint : public std::invalid_argument
        {
            using std::invalid_argument::invalid_argument;
        };

        struct Parsing : public std::invalid_argument
        {
            using std::invalid_argument::invalid_argument;
        };

        struct Timepoint_Not_Found : public std::invalid_argument
        {
            using std::invalid_argument::invalid_argument;
        };

        struct Target_Currency_Not_Found : public std::invalid_argument
        {
            using std::invalid_argument::invalid_argument;
        };

        struct Base_Currency_Not_Found : public std::invalid_argument
        {
            using std::invalid_argument::invalid_argument;
        };

        struct Invalid_Base_Rate : public std::invalid_argument
        {
            using std::invalid_argument::invalid_argument;
        };

        struct Bad_Port : public std::invalid_argument
        {
            using std::invalid_argument::invalid_argument;
        };

    } // namespace Exception
} // namespace MO
