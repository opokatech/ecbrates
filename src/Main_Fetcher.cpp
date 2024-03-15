#include "Data_Loader.hpp"
#include "Logging.hpp"
#include "Urls.hpp"
#include "Version_Info.hpp"
#include "options/Converters.hpp"
#include "options/Options.hpp"

#include <csignal>
#include <iostream>

int main(int argc, char *argv[])
{
    Options::Options options;

    options.add_mandatory("mode", "Fetch historical or daily data (hist or daily)", [](const std::string &param) {
        return param == "hist" || param == "daily"; // NOLINT
    });

    options.add_optional("hist_url", "URL to get historical data", ECB::ECB_URL_HIST, [](const std::string &param) {
        return !param.empty(); // valid data is not empty
    });

    options.add_optional("daily_url", "URL to get daily data", ECB::ECB_URL_DAILY, [](const std::string &param) {
        return !param.empty(); // valid data is not empty
    });

    options.add_optional("save_file", "Save data to a this file", "", [](const std::string &param) {
        return !param.empty(); // valid data is not empty
    });

    options.add_flag("help", "Show help");

    const bool parse_result = options.parse(argc, argv);
    const bool show_help = options.as_bool("help");

    if (!parse_result || show_help)
    {
        std::cout << "ECB data fetcher" << std::endl;
        std::cout << "  Version: " << ECB::Version_Info::HASH << std::endl;
        std::cout << "  Date   : " << ECB::Version_Info::DATE << std::endl;
        std::cout << std::endl;
        std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
        std::cout << options.get_possible_options() << std::endl;

        return -1;
    }

    const bool hist_mode = (options.as_string("mode") == "hist");
    const std::string &url = (hist_mode) ? options.as_string("hist_url") : options.as_string("daily_url");
    const std::string &save_to_file = options.as_string("save_file");

    std::cout << "Getting " << (hist_mode ? "historical" : "daily") << " data"
              << (!save_to_file.empty() ? " and saving to " + save_to_file : "") << std::endl;

    const auto records = ECB::Data_Loader::Load_From_Url(url, save_to_file);

    if (records.empty())
    {
        ECB::Log("Failed to load data from url: %s\n", url.c_str());
        return -1;
    }

    for (const auto &record: records)
        std::cout << record.Get_As_String_Without_Base() << std::endl;

    std::cout << "Got " << records.size() << " records\n";

    return 0;
}
