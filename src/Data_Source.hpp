#pragma once

#include <string>

namespace ECB
{
    struct Data_Source_File: public std::string
    {
        using std::string::string;
    };

    struct Data_Source_Url: public std::string
    {
        using std::string::string;
    };
} // namespace ECB
