#include "gtest/gtest.h"

#include "Rates.hpp"

using namespace ECB;

TEST(Rates, Clear)
{
    Rates rates;
    rates.Add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});
    rates.Clear();
    EXPECT_EQ(0, rates.Count());
}

TEST(Rates, Count)
{
    Rates rates;
    rates.Add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});
    rates.Add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    EXPECT_EQ(2, rates.Count());
}

TEST(Rates, Get_Last)
{
    Rates rates;

    EXPECT_FALSE(rates.Get_Last().has_value());

    rates.Add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    rates.Add(Record{Time_Point{"2020-01-03"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    rates.Add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});

    auto rec = rates.Get_Last();
    EXPECT_TRUE(rec.has_value());
    EXPECT_EQ(rec.value(), Time_Point{"2020-01-03"});
}

TEST(Rates, Add)
{
    Rates rates;
    rates.Add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});
    rates.Add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    EXPECT_EQ(2, rates.Count());

    rates.Add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    EXPECT_EQ(2, rates.Count());
}

TEST(Rates, Get)
{
    Rates rates;

    auto r1 = Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 0.91}, {"GBP", 1.17}}};
    auto r2 = Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 0.93}, {"GBP", 1.20}}};
    rates.Add(r1);
    rates.Add(r2);

    auto rec = rates.Get(r1.Get_Time_Point(), r1.Get_Base());
    EXPECT_TRUE(rec.has_value());
    EXPECT_EQ(rec.value().Get_Time_Point(), r1.Get_Time_Point());
    EXPECT_EQ(rec.value().Get_Base(), r1.Get_Base());

    EXPECT_EQ(rec.value().Get_Rates(), r1.Get_Rates());

    // invalid date
    rec = rates.Get(Time_Point{"2020-01-30"}, r1.Get_Base());
    EXPECT_FALSE(rec.has_value());

    // invalid base
    rec = rates.Get(r1.Get_Time_Point(), "WRONG");
    EXPECT_FALSE(rec.has_value());

    // rebased with valid date
    rec = rates.Get(r2.Get_Time_Point(), "USD");
    EXPECT_TRUE(rec.has_value());

    EXPECT_EQ(rec.value().Get_Time_Point(), r2.Get_Time_Point());
    EXPECT_EQ(rec.value().Get_Base(), "USD");
    std::vector<Currency_Value> r2_usd = {{"EUR", 1.0 / 0.93}, {"GBP", 1.20 / 0.93}};
    EXPECT_EQ(rec.value().Get_Rates(), r2_usd);

    // rebased another currency
    rec = rates.Get(r2.Get_Time_Point(), "GBP");
    EXPECT_TRUE(rec.has_value());

    EXPECT_EQ(rec.value().Get_Time_Point(), r2.Get_Time_Point());
    EXPECT_EQ(rec.value().Get_Base(), "GBP");
    std::vector<Currency_Value> r2_gbp = {{"USD", 0.93 / 1.20}, {"EUR", 1.0 / 1.20}};
    EXPECT_EQ(rec.value().Get_Rates(), r2_gbp);
}
