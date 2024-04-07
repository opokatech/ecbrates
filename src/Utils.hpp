#pragma once

#include <string>
#include <vector>

class Utils
{
public:
    // Split given string using given delimiter.
    static std::vector<std::string> split(const std::string &a_string, char a_delimiter);

    // Makes string uppercase inplace.
    static std::string &uppercase(std::string &a_string);
};
