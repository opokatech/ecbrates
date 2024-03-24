#include "gtest/gtest.h"

#include "Ecb.hpp"

using namespace ECB;

TEST(Ecb, Clear)
{
    Rates ecb;
    ecb.Add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});
    ecb.Clear();
    EXPECT_EQ(0, ecb.Count());
}

TEST(Ecb, Count)
{
    Rates ecb;
    ecb.Add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});
    ecb.Add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    EXPECT_EQ(2, ecb.Count());
}

TEST(Ecb, Get_Last)
{
    Rates ecb;

    EXPECT_FALSE(ecb.Get_Last().has_value());

    ecb.Add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    ecb.Add(Record{Time_Point{"2020-01-03"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    ecb.Add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});

    auto rec = ecb.Get_Last();
    EXPECT_TRUE(rec.has_value());
    EXPECT_EQ(rec.value(), Time_Point{"2020-01-03"});
}

TEST(Ecb, Add)
{
    Rates ecb;
    ecb.Add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});
    ecb.Add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    EXPECT_EQ(2, ecb.Count());

    ecb.Add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    EXPECT_EQ(2, ecb.Count());
}

TEST(Ecb, Get)
{
    Rates ecb;

    auto r1 = Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 0.91}, {"GBP", 1.17}}};
    auto r2 = Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 0.93}, {"GBP", 1.20}}};
    ecb.Add(r1);
    ecb.Add(r2);

    auto rec = ecb.Get(r1.Get_Time_Point(), r1.Get_Base());
    EXPECT_TRUE(rec.has_value());
    EXPECT_EQ(rec.value().Get_Time_Point(), r1.Get_Time_Point());
    EXPECT_EQ(rec.value().Get_Base(), r1.Get_Base());

    EXPECT_EQ(rec.value().Get_Rates(), r1.Get_Rates());

    // invalid date
    rec = ecb.Get(Time_Point{"2020-01-30"}, r1.Get_Base());
    EXPECT_FALSE(rec.has_value());

    // invalid base
    rec = ecb.Get(r1.Get_Time_Point(), "WRONG");
    EXPECT_FALSE(rec.has_value());

    // rebased with valid date
    rec = ecb.Get(r2.Get_Time_Point(), "USD");
    EXPECT_TRUE(rec.has_value());

    EXPECT_EQ(rec.value().Get_Time_Point(), r2.Get_Time_Point());
    EXPECT_EQ(rec.value().Get_Base(), "USD");
    std::vector<Currency_Value> r2_usd = {{"EUR", 1.0 / 0.93}, {"GBP", 1.20 / 0.93}};
    EXPECT_EQ(rec.value().Get_Rates(), r2_usd);

    // rebased another currency
    rec = ecb.Get(r2.Get_Time_Point(), "GBP");
    EXPECT_TRUE(rec.has_value());

    EXPECT_EQ(rec.value().Get_Time_Point(), r2.Get_Time_Point());
    EXPECT_EQ(rec.value().Get_Base(), "GBP");
    std::vector<Currency_Value> r2_gbp = {{"USD", 0.93 / 1.20}, {"EUR", 1.0 / 1.20}};
    EXPECT_EQ(rec.value().Get_Rates(), r2_gbp);
}
