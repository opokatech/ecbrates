#include "Ecb.hpp"
#include "Logging.hpp"
#include "Utils.hpp"

namespace ECB
{
    std::optional<Record> Ecb::Get(const Time_Point &tp, std::optional<Symbol> base) const
    {
        std::scoped_lock lock(m_mutex);

        const auto it = m_data.find(tp);

        if (it == m_data.end())
            return {};

        Record ret = it->second;

        if (ret.Get_Base() == base)
            return ret;

        // different base, so rebase
        if (!ret.Rebase(base.value())) // invalid base so return nothing
            return {};

        // successfully rebased
        return ret;
    }

    // -----------------------------------------------------------------------------------------------------------------

    // Result Ecb::get_data(const Timepoint_Prices &a_tpd, bool a_historical, std::optional<std::vector<Symbol>>
    // a_symbols,
    //                      std::optional<Symbol> a_base) const
    // {
    //     std::scoped_lock lock(m_mutex);

    //     Symbol base = a_base.value_or(DEFAULT_CURRENCY);
    //     std::vector<Symbol> symbols = a_symbols.value_or(std::vector<Symbol>{});

    //     // uppercase base
    //     Utils::uppercase(base);
    //     // uppercase all given symbols
    //     std::for_each(symbols.begin(), symbols.end(), Utils::uppercase);

    //     Result res(a_historical);
    //     res.timepoint = a_tpd.first; // save timepoint
    //     res.base = base;

    //     auto &ret_prices = res.prices;        // alias of target container
    //     const auto &tp_prices = a_tpd.second; // alias of source container

    //     // check if given symbols are in the set
    //     if (base == DEFAULT_CURRENCY)
    //     {
    //         if (symbols.size() > 0) // if we have any symbols then get only those
    //         {
    //             for (const auto &sym: symbols)
    //             {
    //                 if (sym == base)
    //                 {
    //                     ret_prices[base] = 1.0;
    //                     continue;
    //                 }

    //                 if (const auto tp_prices_it = tp_prices.find(sym); tp_prices_it != tp_prices.end())
    //                 {
    //                     ret_prices[tp_prices_it->first] = tp_prices_it->second;
    //                 }
    //                 else
    //                 {
    //                     res.error = Error::INVALID_SYMBOL;
    //                     return res;
    //                 }
    //             }
    //         }
    //         else // if no symbols are passed - get all of them
    //         {
    //             ret_prices = tp_prices;
    //         }
    //     }
    //     else // using different base
    //     {
    //         const auto base_it = tp_prices.find(base);

    //         if (base_it == tp_prices.end()) // not found
    //         {
    //             res.error = Error::INVALID_BASE;
    //             return res;
    //         }

    //         auto base_in_eur = base_it->second;

    //         if (base_in_eur < 0.001)
    //         {
    //             res.error = Error::INVALID_BASE;
    //             return res;
    //         }

    //         // now we can accept EUR too
    //         if (symbols.size() > 0)
    //         {
    //             for (const auto &sym: symbols)
    //             {
    //                 if (sym == base)
    //                 {
    //                     ret_prices[base] = 1.0;
    //                     continue;
    //                 }

    //                 if (sym == DEFAULT_CURRENCY)
    //                 {
    //                     ret_prices[sym] = 1.0 / base_in_eur;
    //                 }
    //                 else if (sym != base)
    //                 {
    //                     if (const auto tp_prices_it = tp_prices.find(sym); tp_prices_it != tp_prices.end())
    //                     {
    //                         ret_prices[sym] = tp_prices_it->second / base_in_eur;
    //                     }
    //                     else
    //                     {
    //                         res.error = Error::INVALID_SYMBOL;
    //                         return res;
    //                     }
    //                 }
    //             }
    //         }
    //         else // get all symbols
    //         {
    //             ret_prices[DEFAULT_CURRENCY] = 1.0 / base_in_eur; // add DEFAULT
    //             for (const auto &[sym, value]: tp_prices)
    //             {
    //                 if (sym != base) // don't add base
    //                 {
    //                     ret_prices[sym] = value / base_in_eur;
    //                 }
    //             }
    //         }
    //     }

    //     return res;
    // }
} // namespace ECB
