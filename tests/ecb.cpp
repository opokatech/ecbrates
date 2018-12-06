#include "gtest/gtest.h"

#include "Ecb.hpp"
#include "Error.hpp"
#include "Symbol.hpp"

using namespace MO;

static const char *DAILY = "data/daily.xml";
static const char *HIST  = "data/hist.xml";

TEST(EcbBasic, LoadingFiles)
{
    Ecb ecb;
    EXPECT_EQ(ecb.Get_Timepoint_Count(), 0);

    ecb.Load(Data_Source_File{"whatever"});
    EXPECT_EQ(ecb.Get_Timepoint_Count(), 0);

    ecb.Load(Data_Source_File{DAILY});
    EXPECT_EQ(ecb.Get_Timepoint_Count(), 1);

    ecb.Clear();
    EXPECT_EQ(ecb.Get_Timepoint_Count(), 0);

    ecb.Load(Data_Source_File{HIST});
    EXPECT_EQ(ecb.Get_Timepoint_Count(), 5101);

    ecb.Load(Data_Source_File{DAILY});
    EXPECT_EQ(ecb.Get_Timepoint_Count(), 5102);
}

TEST(EcbBasic, Latest_No_Base_No_Symbols_But_No_Data_Loaded)
{
    Ecb ecb;
    auto r = ecb.Get_Latest({}, {});

    EXPECT_EQ(r.error, Error::NOT_FOUND);
}

TEST(EcbBasic, Latest_No_Base_No_Symbols)
{
    Ecb ecb{Data_Source_File{DAILY}};

    auto r = ecb.Get_Latest({}, {});
    EXPECT_EQ(r.error, Error::OK);

    EXPECT_EQ(r.timepoint, Timepoint("2018-12-03"));
    EXPECT_NE(r.timepoint, Timepoint("2018-12-15"));

    EXPECT_EQ(r.prices.size(), 32); // we have so many currencies there
    EXPECT_DOUBLE_EQ(r.prices.at("USD"), 1.1332);
    EXPECT_EQ(r.prices.find("EUR"), r.prices.end()); // we should not have base currency in return
}

TEST(EcbBasic, Latest_Default_Base_Uppercase_No_Symbols)
{
    Ecb ecb{Data_Source_File{DAILY}};

    auto r = ecb.Get_Latest({}, {"EUR"});
    EXPECT_EQ(r.error, Error::OK);

    EXPECT_EQ(r.timepoint, Timepoint("2018-12-03"));
    EXPECT_NE(r.timepoint, Timepoint("2018-12-15"));

    EXPECT_EQ(r.prices.size(), 32); // we have so many currencies there
    EXPECT_DOUBLE_EQ(r.prices.at("USD"), 1.1332);
    EXPECT_EQ(r.prices.find("EUR"), r.prices.end()); // we should not have base currency in return
}

TEST(EcbBasic, Latest_Default_Base_Lowercase_No_Symbols)
{
    Ecb ecb{Data_Source_File{DAILY}};

    auto r = ecb.Get_Latest({}, {"eur"});
    EXPECT_EQ(r.error, Error::OK);

    EXPECT_EQ(r.timepoint, Timepoint("2018-12-03"));
    EXPECT_NE(r.timepoint, Timepoint("2018-12-15"));

    EXPECT_EQ(r.prices.size(), 32); // we have so many currencies there
    EXPECT_DOUBLE_EQ(r.prices.at("USD"), 1.1332);
    EXPECT_EQ(r.prices.find("EUR"), r.prices.end()); // we should not have base currency in return
}

TEST(EcbBasic, Latest_Default_Base_And_Base_As_The_Only_Symbol)
{
    Ecb ecb{Data_Source_File{DAILY}};

    auto r = ecb.Get_Latest(Symbols{"eur"}, {});
    EXPECT_EQ(r.error, Error::OK);

    EXPECT_EQ(r.timepoint, Timepoint("2018-12-03"));

    EXPECT_EQ(r.prices.size(), 1); // we have so many currencies there
    EXPECT_DOUBLE_EQ(r.prices.at("EUR"), 1);
}

TEST(EcbBasic, Latest_Invalid_Base_No_Symbols)
{
    Ecb ecb{Data_Source_File{DAILY}};

    auto r = ecb.Get_Latest({}, {"BLABLA"});
    EXPECT_EQ(r.error, Error::INVALID_BASE);
}

TEST(EcbBasic, Latest_Invalid_Base_Some_Symbols)
{
    Ecb ecb{Data_Source_File{DAILY}};

    auto r = ecb.Get_Latest(Symbols{"USD"}, {"BLABLA"});
    EXPECT_EQ(r.error, Error::INVALID_BASE);
}

TEST(EcbBasic, Latest_Other_Base_Uppercase_No_Symbols)
{
    Ecb ecb{Data_Source_File{DAILY}};

    auto r = ecb.Get_Latest({}, {"USD"});
    EXPECT_EQ(r.error, Error::OK);

    EXPECT_EQ(r.timepoint, Timepoint("2018-12-03"));

    EXPECT_EQ(r.prices.size(), 32); // we have so many currencies there
    EXPECT_NEAR(r.prices.at("EUR"), 0.882456, 0.0001);
    ASSERT_EQ(r.prices.find("USD"), r.prices.end()); // we should not have base currency in return
}

TEST(EcbBasic, Latest_Other_Base_Lowercase_No_Symbols)
{
    Ecb ecb{Data_Source_File{DAILY}};

    auto r = ecb.Get_Latest({}, {"usd"});
    EXPECT_EQ(r.error, Error::OK);

    EXPECT_EQ(r.timepoint, Timepoint("2018-12-03"));

    EXPECT_EQ(r.prices.size(), 32); // we have so many currencies there
    EXPECT_NEAR(r.prices.at("EUR"), 0.882456, 0.0001);
    ASSERT_EQ(r.prices.find("USD"), r.prices.end()); // we should not have base currency in return
}

TEST(EcbBasic, Latest_Other_Base_Own_Symbol_Only)
{
    Ecb ecb(Data_Source_File{DAILY});

    // good symbols
    auto r = ecb.Get_Latest(Symbols{"USD"}, "USD");
    EXPECT_EQ(r.error, Error::OK);

    EXPECT_EQ(r.timepoint, Timepoint("2018-12-03"));

    EXPECT_EQ(r.prices.size(), 1);
    EXPECT_NEAR(r.prices.at("usd"), 1, 0.0001);
}

TEST(EcbBasic, Latest_Other_Base_Few_Valid_Symbols)
{
    Ecb ecb(Data_Source_File{DAILY});

    // good symbols
    auto r = ecb.Get_Latest(Symbols{"PLN", "GBP", "EUR"}, "USD");
    EXPECT_EQ(r.error, Error::OK);

    EXPECT_EQ(r.timepoint, Timepoint("2018-12-03"));

    EXPECT_EQ(r.prices.size(), 3);
    EXPECT_NEAR(r.prices.at("PLN"), 3.7777, 0.0001);
    EXPECT_NEAR(r.prices.at("GBP"), 0.7868, 0.0001);
    EXPECT_NEAR(r.prices.at("EUR"), 0.8825, 0.0001);
}

TEST(EcbBasic, Latest_Other_Base_Some_Valid_And_Invalid_Symbols)
{
    Ecb ecb(Data_Source_File{DAILY});
    auto r = ecb.Get_Latest(Symbols{"PLN", "GBP", "bla bla"}, "USD");
    EXPECT_EQ(r.error, Error::INVALID_SYMBOL);
}

TEST(EcbBasic, Latest_Other_Base_Few_Valid_Lowecase_Symbols)
{
    Ecb ecb(Data_Source_File{DAILY});

    // good symbols
    auto r = ecb.Get_Latest(Symbols{"pln", "gbp", "eur"}, "USD");
    EXPECT_EQ(r.error, Error::OK);

    EXPECT_EQ(r.timepoint, Timepoint("2018-12-03"));

    EXPECT_EQ(r.prices.size(), 3);
    EXPECT_NEAR(r.prices.at("PLN"), 3.7777, 0.0001);
    EXPECT_NEAR(r.prices.at("GBP"), 0.7868, 0.0001);
    EXPECT_NEAR(r.prices.at("EUR"), 0.8825, 0.0001);
}

TEST(EcbBasic, Historical_Several_Symbols)
{
    Ecb ecb(Data_Source_File{HIST});

    // good symbols
    auto r = ecb.Get_Historical(Timepoint{"2018-11-14"}, Symbols{"PLN", "GBP", "EUR"}, "USD");
    EXPECT_EQ(r.error, Error::OK);

    EXPECT_EQ(r.timepoint, Timepoint("2018-11-14"));

    EXPECT_EQ(r.prices.size(), 3); // we have so many currencies there
    EXPECT_NEAR(r.prices.at("PLN"), 3.7987, 0.0001);
    EXPECT_NEAR(r.prices.at("GBP"), 0.7706, 0.0001);
    EXPECT_NEAR(r.prices.at("EUR"), 0.8852, 0.0001);

    // bad symbols
    r = ecb.Get_Latest(Symbols{"PLN", "GBP", "bla bla"}, "USD");
    EXPECT_EQ(r.error, Error::INVALID_SYMBOL);
}
