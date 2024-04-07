#include "gtest/gtest.h"

#include "Rates.hpp"

TEST(Rates, Clear)
{
    Rates rates;
    rates.add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});
    rates.clear();
    EXPECT_EQ(0, rates.count());
}

TEST(Rates, Count)
{
    Rates rates;
    rates.add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});
    rates.add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    EXPECT_EQ(2, rates.count());
}

TEST(Rates, Get_Last)
{
    Rates rates;

    EXPECT_FALSE(rates.last().has_value());

    rates.add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    rates.add(Record{Time_Point{"2020-01-03"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    rates.add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});

    auto rec = rates.last();
    EXPECT_TRUE(rec.has_value());
    EXPECT_EQ(rec.value(), Time_Point{"2020-01-03"});
}

TEST(Rates, Add)
{
    Rates rates;
    rates.add(Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 1.0}, {"GBP", 0.9}}});
    rates.add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    EXPECT_EQ(2, rates.count());

    rates.add(Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 1.1}, {"GBP", 0.8}}});
    EXPECT_EQ(2, rates.count());
}

TEST(Rates, Get)
{
    Rates rates;

    auto r1 = Record{Time_Point{"2020-01-01"}, "EUR", {{"USD", 0.91}, {"GBP", 1.17}}};
    auto r2 = Record{Time_Point{"2020-01-02"}, "EUR", {{"USD", 0.93}, {"GBP", 1.20}}};
    rates.add(r1);
    rates.add(r2);

    auto rec = rates.get(r1.time_point(), r1.base());
    EXPECT_TRUE(rec.has_value());
    EXPECT_EQ(rec.value().time_point(), r1.time_point());
    EXPECT_EQ(rec.value().base(), r1.base());

    EXPECT_EQ(rec.value().rates(), r1.rates());

    // invalid date
    rec = rates.get(Time_Point{"2020-01-30"}, r1.base());
    EXPECT_FALSE(rec.has_value());

    // invalid base
    rec = rates.get(r1.time_point(), "WRONG");
    EXPECT_FALSE(rec.has_value());

    // rebased with valid date
    rec = rates.get(r2.time_point(), "USD");
    EXPECT_TRUE(rec.has_value());

    EXPECT_EQ(rec.value().time_point(), r2.time_point());
    EXPECT_EQ(rec.value().base(), "USD");
    std::vector<Currency_Value> r2_usd = {{"EUR", 1.0 / 0.93}, {"GBP", 1.20 / 0.93}};
    EXPECT_EQ(rec.value().rates(), r2_usd);

    // rebased another currency
    rec = rates.get(r2.time_point(), "GBP");
    EXPECT_TRUE(rec.has_value());

    EXPECT_EQ(rec.value().time_point(), r2.time_point());
    EXPECT_EQ(rec.value().base(), "GBP");
    std::vector<Currency_Value> r2_gbp = {{"USD", 0.93 / 1.20}, {"EUR", 1.0 / 1.20}};
    EXPECT_EQ(rec.value().rates(), r2_gbp);
}
