#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "Symbol.hpp"
#include "Utils.hpp"

TEST(Utils, SplitString)
{
    MO::Symbols out;

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
