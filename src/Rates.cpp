#include "Rates.hpp"

std::optional<Record> Rates::get(const Time_Point &tp, std::optional<std::string> base) const
{
    std::scoped_lock lock(m_mutex);

    const auto IT = m_data.find(tp);

    if (IT == m_data.end())
        return {};

    Record ret = IT->second;

    if (!base || ret.base() == base)
        return ret;

    // different base, so rebase
    if (!ret.rebase(base.value())) // invalid base so return nothing
        return {};

    // successfully rebased
    return ret;
}
