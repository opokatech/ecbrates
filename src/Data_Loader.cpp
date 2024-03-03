#include "Data_Loader.hpp"
#include "Logging.hpp"

#include "cpr/cpr.h"
#include "pugixml.hpp"

#include <fstream>

namespace ECB
{
    std::vector<Record> Data_Loader::Load_Data_From_File(const std::string &file_name)
    {
        try
        {
            std::ifstream file(file_name);
            std::string content(std::istreambuf_iterator<char>{file}, {});
            file.close();

            return load_data_from_xml_string(content);
        }
        catch (...)
        {
            ECB::Log("Error loading from a file '%s'\n", file_name.c_str());
        }
        return {};
    }

    std::vector<Record> Data_Loader::Load_Data_From_Url(const std::string &url)
    {
        try
        {
            auto r = cpr::Get(cpr::Url{url}, cpr::VerifySsl{false});

            if (r.status_code == 200)
            {
                return load_data_from_xml_string(r.text);
            }
            else
            {
                ECB::Log("Failed getting data from url '%s', status code = %d\n", url.c_str(), r.status_code);
            }
        }
        catch (...)
        {
            ECB::Log("Error loading from url '%s'\n", url.c_str());
        }
        return {};
    }

    std::vector<Record> Data_Loader::load_data_from_xml_string(const std::string &data)
    {
        std::vector<Record> records;

        pugi::xml_document doc;

        // std::cout << "Parsing historical data from file '" << file_name << "'... " << std::flush;
        // auto time_start = std::chrono::steady_clock::now();
        // auto parse_result = doc.load_file(file_name.c_str());
        auto parse_result = doc.load_string(data.c_str());
        // auto time_end = std::chrono::steady_clock::now();
        // auto parse_duration = std::chrono::duration<double>(time_end - time_start);
        // std::cout << "done (time: " << parse_duration.count() << "). Result: " << parse_result.description() <<
        // std::endl;

        if (!parse_result)
        {
            ECB::Log("Error parsing xml: %s\n", parse_result.description());
            return records;
        }

        auto root = doc.first_child();  // "gesmes:Envelope" is the name
        auto cube = root.child("Cube"); // root of all currency data

        // data parsed, so add it to the local map
        // std::scoped_lock lock(m_mutex);

        const Symbol BASE{"EUR"};
        for (const auto &day_node: cube.children()) // each child is also "Cube"
        {
            std::string time = day_node.attribute("time").as_string();

            Time_Point tp{time};
            std::vector<Currency_Value> rates;

            for (const auto &symbol_node: day_node.children())
            {
                if (strcmp(symbol_node.name(), "Cube") == 0)
                {
                    const Currency_Value cv{Symbol{symbol_node.attribute("currency").as_string()},
                                            symbol_node.attribute("rate").as_double()};
                    rates.emplace_back(cv);
                }
            }

            records.emplace_back(tp, BASE, rates);
        }

        return records;
    }
}; // namespace ECB
