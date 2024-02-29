#include "gtest/gtest.h"

#include "Time_Point.hpp"

using namespace ECB;

TEST(Time_Point, Assigning_Invalid_Data_Results_In_Empty_Time_Point)
{
    Time_Point tp{""};
    EXPECT_FALSE(tp.is_initialized());

    EXPECT_EQ(std::string{""}, static_cast<std::string>(tp));
}
