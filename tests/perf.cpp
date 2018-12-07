#include <chrono>
#include <iostream>

#include "gtest/gtest.h"

#include "Ecb.hpp"
#include "Error.hpp"
#include "Symbol.hpp"

using namespace MO;

static const char *DAILY = "data/daily.xml";

TEST(Performance, Latest_Other_Base_Many_Symbols)
{
    Ecb ecb(Data_Source_File{DAILY});

    Symbols symbols{"jpy", "bgn", "czk", "dkk", "huf", "pln", "ron", "sek", "isk", "nok", "hrk", "try"};

    constexpr unsigned COUNT = 100000;

    auto start = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < COUNT; ++i)
    {
        auto r = ecb.Get_Latest(symbols, "USD");
        EXPECT_EQ(r.error, Error::OK);
        EXPECT_EQ(r.timepoint, Timepoint("2018-12-03"));

        EXPECT_EQ(r.prices.size(), symbols.size());
    }
    auto stop = std::chrono::steady_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "time: " << time.count() << "ms" << std::endl;
}
