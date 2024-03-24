#include "Rates.hpp"
#include "Logging.hpp"

namespace ECB
{
    std::optional<Record> Rates::Get(const Time_Point &tp, std::optional<Symbol> base) const
    {
        std::scoped_lock lock(m_mutex);

        const auto it = m_data.find(tp);

        if (it == m_data.end())
            return {};

        Record ret = it->second;

        if (!base || ret.Get_Base() == base)
            return ret;

        // different base, so rebase
        if (!ret.Rebase(base.value())) // invalid base so return nothing
            return {};

        // successfully rebased
        return ret;
    }
} // namespace ECB
