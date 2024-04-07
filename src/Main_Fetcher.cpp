#include "Data_Loader.hpp"
#include "Log.hpp"
#include "Urls.hpp"
#include "Version_Info.hpp"

#include "options/Converters.hpp"
#include "options/Options.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    using std::cout, std::endl;

    Options::Options options;

    options.add_mandatory("mode", "Fetch historical or daily data (hist or daily)", [](const std::string &param) {
        return param == "hist" || param == "daily"; // NOLINT
    });

    options.add_optional("hist_url", "URL to get historical data", ECB_URL_HIST, [](const std::string &param) {
        return !param.empty(); // valid data is not empty
    });

    options.add_optional("daily_url", "URL to get daily data", ECB_URL_DAILY, [](const std::string &param) {
        return !param.empty(); // valid data is not empty
    });

    options.add_optional("save_file", "Save data to a this file", "", [](const std::string &param) {
        return !param.empty(); // valid data is not empty
    });

    options.add_optional("log_file", "Log file", "", [](const std::string &param) {
        return !param.empty(); // valid data is not empty
    });

    options.add_optional("log_level", "Log level (off, info, warn, error)", "info", [](const std::string &param) {
        return param == "off" || param == "info" || param == "warn" || param == "error";
    });

    options.add_flag("help", "Show help");

    const bool parse_result = options.parse(argc, argv);
    const bool show_help = options.as_bool("help");

    if (!parse_result || show_help)
    {
        cout << "ECB data fetcher" << endl;
        cout << "  Version: " << Version_Info::HASH << endl;
        cout << "  Date   : " << Version_Info::DATE << endl;
        cout << endl;
        cout << "Usage: " << argv[0] << " [options]" << endl;
        cout << options.get_possible_options() << endl;

        return -1;
    }

    Log::init(options.as_string("log_file"));
    Log::setLevel(options.as_string("log_level"));

    const bool hist_mode = (options.as_string("mode") == "hist");
    const std::string &url = (hist_mode) ? options.as_string("hist_url") : options.as_string("daily_url");
    const std::string &save_to_file = options.as_string("save_file");

    Log::info("Getting {} data{}", hist_mode ? "historical" : "daily",
              !save_to_file.empty() ? std::string(" and saving to ") + save_to_file : "");

    const auto records = Data_Loader::load_from_url(url, save_to_file);

    if (records.empty())
    {
        Log::error("Failed to load data from url: {}\n", url);
        return -1;
    }

    for (const auto &record: records)
        cout << record.as_string_without_base() << endl;

    Log::info("Got {} records", records.size());

    return 0;
}
