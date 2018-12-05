#include <functional>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "Ecb.hpp"
#include "Error.hpp"
#include "Exceptions.hpp"
#include "Symbol.hpp"
#include "test_utils.hpp"

TEST(EcbBasic, LoadingFiles)
{
    MO::Ecb ecb;

    EXPECT_EQ(ecb.Get_Timepoint_Count(), 0);

    ecb.Load(MO::Data_Source_File{"data/daily.xml"});
    EXPECT_EQ(ecb.Get_Timepoint_Count(), 1);

    ecb.Clear();
    EXPECT_EQ(ecb.Get_Timepoint_Count(), 0);

    ecb.Load(MO::Data_Source_File{"data/hist.xml"});
    EXPECT_EQ(ecb.Get_Timepoint_Count(), 5101);

    ecb.Load(MO::Data_Source_File{"data/daily.xml"});
    EXPECT_EQ(ecb.Get_Timepoint_Count(), 5102);
}

TEST(EcbBasic, SimpleConversions)
{
    using namespace MO;

    Ecb ecb;

    ecb.Load(MO::Data_Source_File{"data/hist.xml"});

    EXPECT_DOUBLE_EQ(1.1296, ecb.Rate(Timepoint{"2018-11-14"}, Symbol{"USD"}));
    EXPECT_DOUBLE_EQ(1, ecb.Rate(Timepoint{"2018-11-14"}, Symbol{"USD"}, Symbol{"USD"}));

    EXPECT_DOUBLE_EQ(128.64, ecb.Rate(Timepoint{"2018-11-14"}, Symbol{"JPY"}));
    EXPECT_DOUBLE_EQ(4.2911, ecb.Rate(Timepoint{"2018-11-14"}, Symbol{"PLN"}));

    // value of PLN in USD
    EXPECT_DOUBLE_EQ(4.2911 / 1.1296, ecb.Rate(Timepoint{"2018-11-14"}, Symbol{"PLN"}, Symbol{"USD"}));
}

TEST(EcbBasic, Latest_Simple)
{
    using namespace MO;

    Ecb ecb;

    auto r = ecb.Get_Latest({}, {});
    EXPECT_EQ(r.error, MO::Error::NOT_FOUND);

    ecb.Load(MO::Data_Source_File{"data/daily.xml"});

    r = ecb.Get_Latest({}, {});
    EXPECT_EQ(r.error, MO::Error::OK);

    EXPECT_EQ(r.timepoint, MO::Timepoint("2018-12-03"));
    EXPECT_NE(r.timepoint, MO::Timepoint("2018-12-15"));

    EXPECT_EQ(r.prices.size(), 32); // we have so many currencies there
    EXPECT_DOUBLE_EQ(r.prices.at("USD"), 1.1332);
    EXPECT_EQ(r.prices.find("EUR"), r.prices.end()); // we should not have base currency in return
}

TEST(EcbBasic, Latest_Base)
{
    using namespace MO;

    Ecb ecb;

    ecb.Load(MO::Data_Source_File{"data/daily.xml"});

    // bad base currency
    auto r = ecb.Get_Latest({}, {"BLABLA"});
    EXPECT_EQ(r.error, MO::Error::INVALID_BASE);

    // good base currency
    r = ecb.Get_Latest({}, {"USD"});
    EXPECT_EQ(r.error, MO::Error::OK);

    EXPECT_EQ(r.timepoint, MO::Timepoint("2018-12-03"));

    EXPECT_EQ(r.prices.size(), 32); // we have so many currencies there
    EXPECT_NEAR(r.prices.at("EUR"), 0.882456, 0.0001);
    ASSERT_EQ(r.prices.find("USD"), r.prices.end()); // we should not have base currency in return
}

TEST(EcbBasic, Latest_Several_Symbols)
{
    using namespace MO;

    Ecb ecb;

    ecb.Load(MO::Data_Source_File{"data/daily.xml"});

    // good symbols
    auto r = ecb.Get_Latest(Symbols{"PLN", "GBP", "EUR"}, "USD");
    EXPECT_EQ(r.error, MO::Error::OK);

    EXPECT_EQ(r.timepoint, MO::Timepoint("2018-12-03"));

    EXPECT_EQ(r.prices.size(), 3); // we have so many currencies there
    EXPECT_NEAR(r.prices.at("PLN"), 3.7777, 0.0001);
    EXPECT_NEAR(r.prices.at("GBP"), 0.7868, 0.0001);
    EXPECT_NEAR(r.prices.at("EUR"), 0.8825, 0.0001);

    // bad symbols
    r = ecb.Get_Latest(Symbols{"PLN", "GBP", "bla bla"}, "USD");
    EXPECT_EQ(r.error, MO::Error::INVALID_SYMBOL);
}

TEST(EcbBasic, Historical_Several_Symbols)
{
    using namespace MO;

    Ecb ecb;

    ecb.Load(MO::Data_Source_File{"data/hist.xml"});

    // good symbols
    auto r = ecb.Get_Historical(Timepoint{"2018-11-14"}, Symbols{"PLN", "GBP", "EUR"}, "USD");
    EXPECT_EQ(r.error, MO::Error::OK);

    EXPECT_EQ(r.timepoint, MO::Timepoint("2018-11-14"));

    EXPECT_EQ(r.prices.size(), 3); // we have so many currencies there
    EXPECT_NEAR(r.prices.at("PLN"), 3.7987, 0.0001);
    EXPECT_NEAR(r.prices.at("GBP"), 0.7706, 0.0001);
    EXPECT_NEAR(r.prices.at("EUR"), 0.8852, 0.0001);

    // bad symbols
    r = ecb.Get_Latest(Symbols{"PLN", "GBP", "bla bla"}, "USD");
    EXPECT_EQ(r.error, MO::Error::INVALID_SYMBOL);
}

TEST(EcbBasic, InvalidConversions)
{
    using namespace MO;

    Ecb ecb;

    ecb.Load(MO::Data_Source_File{"data/daily.xml"});

    MO::test_exception<Exception::Bad_Timepoint>([&]() { ecb.Rate(Timepoint{"whatever"}, Symbol{"USD"}); });

    MO::test_exception<Exception::Timepoint_Not_Found>([&]() { ecb.Rate(Timepoint{"1234-12-12"}, Symbol{"USD"}); });

    MO::test_exception<Exception::Target_Currency_Not_Found>(
        [&]() { ecb.Rate(Timepoint{"2018-12-03"}, Symbol{"BLABLA"}); });

    MO::test_exception<Exception::Base_Currency_Not_Found>(
        [&]() { ecb.Rate(Timepoint{"2018-12-03"}, Symbol{"PLN"}, Symbol{"ABC"}); });
}
