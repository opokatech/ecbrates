#pragma once

#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "Data_Source.hpp"
#include "Error.hpp"
#include "Prices.hpp"
#include "Result.hpp"
#include "Timepoint.hpp"

namespace MO
{
    class Ecb {
    public:
        using Timepoint_Prices = std::pair<Timepoint, Prices>;

        /// Load data from a file.
        void Load(const Data_Source_File &a_filename) { load_file(a_filename); }

        /// Load data from the network via url.
        void Load(const Data_Source_Url &a_url) { load_url(a_url); }

        /// Clears the local database.
        void Clear()
        {
            std::scoped_lock lock(m_mutex);
            m_data.clear();
        }

        /// Returns number of timepoints stored.
        size_t Get_Timepoint_Count() const
        {
            std::scoped_lock lock(m_mutex);
            return m_data.size();
        }

        /// Gets single rate
        double Rate(const Timepoint &a_tp, const Symbol &a_target, const Symbol &a_base = DEFAULT_CURRENCY) const;

        /// Get full result set for the latest data.
        Result Get_Latest(std::optional<std::vector<Symbol>>, std::optional<Symbol>) const;

        /// Get full result set for the data from given timepoint.
        Result Get_Historical(const Timepoint &, std::optional<std::vector<Symbol>>, std::optional<Symbol>) const;

    private:
        /// Common function used by Get_Latest and Get_Historical.
        Result get_data(const Timepoint_Prices &, bool a_historical, std::optional<std::vector<Symbol>>,
                        std::optional<Symbol>) const;
        /** Loads data from a file.
         *
         * Loads content of a file and calls load_data_from_string.
         */
        void load_file(const std::string &a_filename);

        /** Loads data from given url.
         *
         * Loads content from the url and calls load_data_from_string.
         */
        void load_url(const std::string &a_url);

        /** Loads data from given xml string.
         *
         * If data at given timepoint is already there - it will be overwritten.
         */
        void load_data_from_string(const std::string &a_data);

        std::map<Timepoint, Prices> m_data;
        mutable std::recursive_mutex m_mutex;

        const char *CUBE{"Cube"};
        const char *TIME{"time"};
        const char *CURRENCY{"currency"};
        const char *RATE{"rate"};
    };
} // namespace MO
