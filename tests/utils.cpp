#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "Utils.hpp"

TEST(Utils, SplitString)
{
    std::vector<std::string> out;

    out = MO::Utils::split("", ' ');
    ASSERT_EQ(out.size(), 0);

    out = MO::Utils::split("", ',');
    ASSERT_EQ(out.size(), 0);

    out = MO::Utils::split("a", ',');
    ASSERT_EQ(out.size(), 1);
    ASSERT_EQ(out[0], "a");

    out = MO::Utils::split("b,", ',');
    ASSERT_EQ(out.size(), 1);
    ASSERT_EQ(out[0], "b");

    out = MO::Utils::split("usd,eur,pln", ',');
    ASSERT_EQ(out.size(), 3);
    ASSERT_EQ(out[0], "usd");
    ASSERT_EQ(out[1], "eur");
    ASSERT_EQ(out[2], "pln");
}

TEST(Utils, Uppercase)
{
    std::string s;

    s = "abc";
    ASSERT_EQ(MO::Utils::uppercase(s), "ABC");

    s = "";
    ASSERT_EQ(MO::Utils::uppercase(s), "");

    s = "123";
    ASSERT_EQ(MO::Utils::uppercase(s), "123");

    s = "AbC";
    ASSERT_EQ(MO::Utils::uppercase(s), "ABC");
}
