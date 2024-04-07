#include "gtest/gtest.h"

#include "Time_Point.hpp"

#include <vector>

TEST(Time_Point, Assigning_Invalid_Data_Results_In_Empty_Time_Point)
{
    // clang-format off
    const std::vector<std::string> invalid_strings = {
        "",
        "2020-01-01 01:00:00",
        "2020-01-01-01",
        "2020-01-01-01-01",
        "2020-0",
        "1234-56_78",
        "yyyy-mm-dd"};
    // clang-format on

    const std::string empty_string;
    for (const auto &inv: invalid_strings)
    {
        Time_Point tp{inv};
        EXPECT_FALSE(tp.is_set());
        EXPECT_EQ(empty_string, static_cast<std::string>(tp));
    }
}

TEST(Time_Point, Assigning_Valid_Data_Results_In_Non_Empty_Time_Point)
{
    // clang-format off
    const std::vector<std::string> valid_strings = {
        "2020-01-01",
        "1234-56-78", // no check is made regarding the date
        "2020-02-29"};
    // clang-format on

    for (const auto &inv: valid_strings)
    {
        Time_Point tp{inv};
        EXPECT_TRUE(tp.is_set());
        EXPECT_EQ(inv, static_cast<std::string>(tp));
    }
}
