#pragma once

#include "Record.hpp"

#include <map>
#include <mutex>
#include <optional>
#include <vector>

namespace ECB
{
    /// An object of this class stores exchange rates for different currencies at different time points and allows easy
    /// retrieval. A single record consists of a time point, base and a set of values for different currencies. Records
    /// can be added to this object and returned in with a different base or with different currencies. Only a single
    /// record is always returned.
    class Ecb
    {
    public:
        /// Clears the local database.
        void Clear()
        {
            std::scoped_lock lock(m_mutex);
            m_data.clear();
        }

        /// Returns number of stored records.
        size_t Count() const
        {
            std::scoped_lock lock(m_mutex);
            return m_data.size();
        }

        std::optional<Time_Point> Get_Last() const
        {
            std::scoped_lock lock(m_mutex);
            if (m_data.empty())
                return {};
            return m_data.rbegin()->first;
        }

        /// Adds a new record to the local database.
        void Add(const Record &a_record)
        {
            std::scoped_lock lock(m_mutex);
            m_data[a_record.Get_Time_Point()] = a_record;
        }

        /// Adds a vector of records to the local database.
        void Add(const std::vector<Record> &a_records)
        {
            std::scoped_lock lock(m_mutex);
            for (const auto &rec: a_records)
                m_data[rec.Get_Time_Point()] = rec;
        }

        /// Returns a record for the given time point. If the base is given, the record is rebased to the given base.
        std::optional<Record> Get(const Time_Point &tp, std::optional<Symbol> base) const;

    private:
        std::map<Time_Point, Record> m_data;
        mutable std::recursive_mutex m_mutex;
    };
} // namespace ECB
