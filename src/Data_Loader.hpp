#pragma once

#include "Record.hpp"

#include <vector>

class Data_Loader
{
public:
    Data_Loader() = delete;

    // Load data from a file and return a vector of records.
    // Returns an empty vector if the data could not be loaded.
    static std::vector<Record> load_from_file(const std::string &file_name);

    // Load data from a url and return a vector of records.
    // Returns an empty vector if the data could not be loaded.
    static std::vector<Record> load_from_url(const std::string &url, std::string_view save_to_file = "");

private:
    // Load data from a string and return a vector of records.
    static std::vector<Record> load_from_xml_string(const std::string &data);
};
