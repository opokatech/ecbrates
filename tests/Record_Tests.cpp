#include "gtest/gtest.h"

#include "Record.hpp"

using namespace ECB;

TEST(Record, AlmostEmpty)
{
    Record rec(Time_Point("1234-56-78"), "EUR", {});

    EXPECT_EQ(static_cast<std::string>(rec.Get_Time_Point()), "1234-56-78");
    EXPECT_EQ(rec.Get_Base(), "EUR");
    EXPECT_EQ(rec.Get_Rates().size(), 0);

    EXPECT_EQ(rec.Get_As_String_With_Base(), "1234-56-78(EUR):");
    EXPECT_EQ(rec.Get_As_String_Without_Base(), "1234-56-78:");
}

TEST(Record, FewDataPoints)
{
    Record rec(Time_Point("1234-56-78"), "EUR", {{"USD", 1.23}, {"GBP", 2.34}});

    EXPECT_EQ(static_cast<std::string>(rec.Get_Time_Point()), "1234-56-78");
    EXPECT_EQ(rec.Get_Base(), "EUR");
    EXPECT_EQ(rec.Get_Rates().size(), 2);

    EXPECT_EQ(rec.Get_As_String_With_Base(), "1234-56-78(EUR): USD(1.23) GBP(2.34)");
    EXPECT_EQ(rec.Get_As_String_Without_Base(), "1234-56-78: USD(1.23) GBP(2.34)");
}
