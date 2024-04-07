#pragma once

#include "Record.hpp"

#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

// An object of this class stores exchange rates for different currencies at different time points and allows easy
// retrieval. A single record consists of a time point, base and a set of values for different currencies. Records
// can be added to this object and returned in with a different base or with different currencies. Only a single
// record is always returned.
class Rates
{
public:
    // Clears the local database.
    void clear()
    {
        std::scoped_lock lock(m_mutex);
        m_data.clear();
    }

    // Returns number of stored records.
    size_t count() const
    {
        std::scoped_lock lock(m_mutex);
        return m_data.size();
    }

    std::optional<Time_Point> last() const
    {
        std::scoped_lock lock(m_mutex);
        if (m_data.empty())
            return {};
        return m_data.rbegin()->first;
    }

    // Adds a new record to the local database.
    void add(const Record &a_record)
    {
        std::scoped_lock lock(m_mutex);
        m_data[a_record.time_point()] = a_record;
    }

    // Adds a vector of records to the local database.
    void add(const std::vector<Record> &a_records)
    {
        std::scoped_lock lock(m_mutex);
        for (const auto &rec: a_records)
            m_data[rec.time_point()] = rec;
    }

    // Returns a record for the given time point. If the base is given, the record is rebased to the given base.
    std::optional<Record> get(const Time_Point &tp, std::optional<std::string> base) const;

private:
    std::map<Time_Point, Record> m_data;
    mutable std::recursive_mutex m_mutex;
};
