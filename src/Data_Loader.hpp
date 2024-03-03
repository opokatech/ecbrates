#pragma once

#include "Record.hpp"

#include <vector>

namespace ECB
{
    class Data_Loader
    {
    public:
        /// Load data from a file and return a vector of records.
        /// Returns an empty vector if the data could not be loaded.
        static std::vector<Record> Load_Data_From_File(const std::string &file_name);

        /// Load data from a url and return a vector of records.
        /// Returns an empty vector if the data could not be loaded.
        static std::vector<Record> Load_Data_From_Url(const std::string &url);

    private:
        /// Load data from a string and return a vector of records.
        static std::vector<Record> load_data_from_xml_string(const std::string &data);
    };

} // namespace ECB
