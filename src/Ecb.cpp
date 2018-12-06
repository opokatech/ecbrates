#include <cstring>
#include <fstream>
#include <iostream>

#include "cpr/cpr.h"
#include "pugixml.hpp"

#include "Ecb.hpp"
#include "Exceptions.hpp"
#include "Logging.hpp"
#include "Symbol.hpp"
#include "Timepoint.hpp"

namespace MO
{
    Result Ecb::Get_Latest(std::optional<std::vector<Symbol>> a_symbols, std::optional<Symbol> a_base) const
    {
        const auto last_dt = m_data.rbegin();

        if (last_dt == m_data.rend())
        {
            return Result{false, Error::NOT_FOUND};
        }

        return get_data(*last_dt, false, a_symbols, a_base);
    }

    // -----------------------------------------------------------------------------------------------------------------

    Result Ecb::Get_Historical(const Timepoint &a_tp, std::optional<std::vector<Symbol>> a_symbols,
                               std::optional<Symbol> a_base) const
    {
        std::scoped_lock lock(m_mutex);

        const auto tp_data_it = m_data.find(a_tp);

        if (tp_data_it == m_data.end())
        {
            return Result{true, Error::NOT_FOUND};
        }

        return get_data(*tp_data_it, true, a_symbols, a_base);
    }

    // -----------------------------------------------------------------------------------------------------------------

    Result Ecb::get_data(const Timepoint_Prices &a_tpd, bool a_historical, std::optional<std::vector<Symbol>> a_symbols,
                         std::optional<Symbol> a_base) const
    {
        std::scoped_lock lock(m_mutex);

        const Symbol &base                 = a_base.value_or(DEFAULT_CURRENCY);
        const std::vector<Symbol> &symbols = a_symbols.value_or(std::vector<Symbol>{});

        Result res(a_historical);
        res.timepoint = a_tpd.first; // save timepoint
        res.base      = base;

        auto &ret_prices      = res.prices;   // alias of target container
        const auto &tp_prices = a_tpd.second; // alias of source container

        // check if given symbols are in the set
        if (base == DEFAULT_CURRENCY)
        {
            if (symbols.size() > 0) // if we have any symbols then get only those
            {
                for (const auto &sym : symbols)
                {
                    if (sym == base)
                    {
                        ret_prices[base] = 1.0;
                        continue;
                    }

                    if (const auto tp_prices_it = tp_prices.find(sym); tp_prices_it != tp_prices.end())
                    {
                        ret_prices[tp_prices_it->first] = tp_prices_it->second;
                    }
                    else
                    {
                        res.error = Error::INVALID_SYMBOL;
                        return res;
                    }
                }
            }
            else // if no symbols are passed - get all of them
            {
                ret_prices = tp_prices;
            }
        }
        else // using different base
        {
            const auto base_it = tp_prices.find(base);

            if (base_it == tp_prices.end()) // not found
            {
                res.error = Error::INVALID_BASE;
                return res;
            }

            auto base_in_eur = base_it->second;

            if (base_in_eur < 0.001)
            {
                res.error = Error::INVALID_BASE;
                return res;
            }

            // now we can accept EUR too
            if (symbols.size() > 0)
            {
                for (const auto &sym : symbols)
                {
                    if (sym == base)
                    {
                        ret_prices[base] = 1.0;
                        continue;
                    }

                    if (sym == DEFAULT_CURRENCY)
                    {
                        ret_prices[sym] = 1.0 / base_in_eur;
                    }
                    else if (sym != base)
                    {
                        if (const auto tp_prices_it = tp_prices.find(sym); tp_prices_it != tp_prices.end())
                        {
                            ret_prices[sym] = tp_prices_it->second / base_in_eur;
                        }
                        else
                        {
                            res.error = Error::INVALID_SYMBOL;
                            return res;
                        }
                    }
                }
            }
            else // get all symbols
            {
                ret_prices[DEFAULT_CURRENCY] = 1.0 / base_in_eur; // add DEFAULT
                for (const auto &[sym, value] : tp_prices)
                {
                    if (sym != base) // don't add base
                    {
                        ret_prices[sym] = value / base_in_eur;
                    }
                }
            }
        }

        return res;
    }

    // -----------------------------------------------------------------------------------------------------------------

    void Ecb::load_file(const std::string &a_filename)
    {
        try
        {
            std::ifstream file(a_filename);
            std::string content(std::istreambuf_iterator<char>{file}, {});
            file.close();

            load_data_from_string(content);
        }
        catch (...)
        {
            MO::Log("Error loading from a file '%s'\n", a_filename.c_str());
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    void Ecb::load_url(const std::string &a_url)
    {
        try
        {
            auto r = cpr::Get(cpr::Url{a_url}, cpr::VerifySsl{false});

            if (r.status_code == 200)
            {
                load_data_from_string(r.text);
            }
            else
            {
                MO::Log("Failed getting data from url '%s', status code = %d\n", a_url.c_str(), r.status_code);
            }
        }
        catch (...)
        {
            MO::Log("Error loading from url '%s'\n", a_url.c_str());
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    void Ecb::load_data_from_string(const std::string &a_data)
    {
        pugi::xml_document doc;

        // std::cout << "Parsing historical data from file '" << file_name << "'... " << std::flush;
        // auto time_start = std::chrono::steady_clock::now();
        // auto parse_result = doc.load_file(file_name.c_str());
        auto parse_result = doc.load_string(a_data.c_str());
        // auto time_end = std::chrono::steady_clock::now();
        // auto parse_duration = std::chrono::duration<double>(time_end - time_start);
        // std::cout << "done (time: " << parse_duration.count() << "). Result: " << parse_result.description() <<
        // std::endl;

        if (!parse_result)
        {
            throw Exception::Parsing(std::string("error parsing xml:") + parse_result.description());
        }

        auto root = doc.first_child(); // gesmes:Envelope is the name
        auto cube = root.child(CUBE);  // root of all currency data

        // data parsed, so add it to the local map
        std::scoped_lock lock(m_mutex);
        for (const auto &day_node : cube.children()) // each child is also "Cube"
        {
            std::string time = day_node.attribute(TIME).as_string();

            Timepoint tp{time};
            Prices prices;

            for (const auto &sym_node : day_node.children())
            {
                if (strcmp(sym_node.name(), CUBE) == 0)
                {
                    auto symbol = Symbol{sym_node.attribute(CURRENCY).as_string()};
                    auto rate   = sym_node.attribute(RATE).as_double();

                    prices[symbol] = rate;
                }
            }

            m_data[tp] = prices;
        }
    }

} // namespace MO
