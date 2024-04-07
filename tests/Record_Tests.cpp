#include "gtest/gtest.h"

#include "Record.hpp"

TEST(Record, AlmostEmpty)
{
    Record rec(Time_Point("1234-56-78"), "EUR", {});

    EXPECT_EQ(static_cast<std::string>(rec.time_point()), "1234-56-78");
    EXPECT_EQ(rec.base(), "EUR");
    EXPECT_EQ(rec.rates().size(), 0);

    EXPECT_EQ(rec.as_string_with_base(), "1234-56-78(EUR):");
    EXPECT_EQ(rec.as_string_without_base(), "1234-56-78:");
}

TEST(Record, FewDataPoints)
{
    Record rec(Time_Point("1234-56-78"), "EUR", {{"USD", 1.23}, {"GBP", 2.34}});

    EXPECT_EQ(static_cast<std::string>(rec.time_point()), "1234-56-78");
    EXPECT_EQ(rec.base(), "EUR");
    EXPECT_EQ(rec.rates().size(), 2);

    EXPECT_EQ(rec.as_string_with_base(), "1234-56-78(EUR): USD(1.23) GBP(2.34)");
    EXPECT_EQ(rec.as_string_without_base(), "1234-56-78: USD(1.23) GBP(2.34)");
}

TEST(Record, Rebase)
{
    Record rec(Time_Point("1234-56-78"), "EUR", {{"USD", 1.23}, {"GBP", 2.34}});

    rec.rebase("USD");

    EXPECT_EQ(static_cast<std::string>(rec.time_point()), "1234-56-78"); // no change
    EXPECT_EQ(rec.base(), "USD");                                        // changed
    EXPECT_EQ(rec.rates().size(), 2);                                    // same size

    EXPECT_EQ(rec.as_string_with_base(), "1234-56-78(USD): EUR(0.81) GBP(1.90)");
    EXPECT_EQ(rec.as_string_without_base(), "1234-56-78: EUR(0.81) GBP(1.90)");
}
