#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "Symbol.hpp"
#include "Utils.hpp"

TEST(Utils, SplitString)
{
    ECB::Symbols out;

    out = ECB::Utils::split("", ' ');
    ASSERT_EQ(out.size(), 0);

    out = ECB::Utils::split("", ',');
    ASSERT_EQ(out.size(), 0);

    out = ECB::Utils::split("a", ',');
    ASSERT_EQ(out.size(), 1);
    ASSERT_EQ(out[0], "a");

    out = ECB::Utils::split("b,", ',');
    ASSERT_EQ(out.size(), 1);
    ASSERT_EQ(out[0], "b");

    out = ECB::Utils::split("usd,eur,pln", ',');
    ASSERT_EQ(out.size(), 3);
    ASSERT_EQ(out[0], "usd");
    ASSERT_EQ(out[1], "eur");
    ASSERT_EQ(out[2], "pln");
}
