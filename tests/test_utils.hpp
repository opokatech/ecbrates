#pragma once

#include "gtest/gtest.h"
#include <typeinfo>

namespace MO
{
    template <typename Expected_Exception> void test_exception(std::function<void()> a_code_to_fail)
    {
        try
        {
            a_code_to_fail();
            FAIL();
        }
        catch (Expected_Exception const &)
        {
        }
        catch (const std::exception &a_ex)
        {
            FAIL() << "Expected exception: " << typeid(Expected_Exception).name()
                   << ", but got this: " << typeid(a_ex).name();
        }
    }
} // namespace MO
