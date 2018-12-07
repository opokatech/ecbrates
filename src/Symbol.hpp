#pragma once

#include <string>
#include <vector>

namespace MO
{
    /* Using special char traits is nice - the string behaves like case insensitive,
     * however each time we compare strings we pay for this convenience.
     * On my machine performing 100k request for some set of latest data took
     * 3.3s when using Case_Insensitive_Char_Traits
     * vs
     * 2.9s when using plain std::string and uppercasing strings passed to Ecb::Get*.
     *
     * I leave the code here commented out with this comment
     * to be able to test it quickly again if needs to be.

    struct Case_Insensitive_Char_Traits : public std::char_traits<char>
    {
        static char to_upper(char ch) { return std::toupper((unsigned char)ch); }
        static bool eq(char c1, char c2) { return to_upper(c1) == to_upper(c2); }
        static bool lt(char c1, char c2) { return to_upper(c1) < to_upper(c2); }
        static int compare(const char *s1, const char *s2, size_t n)
        {
            while (n-- != 0)
            {
                if (to_upper(*s1) < to_upper(*s2))
                    return -1;
                if (to_upper(*s1) > to_upper(*s2))
                    return 1;
                ++s1;
                ++s2;
            }
            return 0;
        }
        static const char *find(const char *s, int n, char a)
        {
            auto const ua(to_upper(a));
            while (n-- != 0)
            {
                if (to_upper(*s) == ua)
                    return s;
                s++;
            }
            return nullptr;
        }
    };

    using Symbol  = std::basic_string<char, Case_Insensitive_Char_Traits>;
    */
    using Symbol  = std::string;
    using Symbols = std::vector<Symbol>;

    static const Symbol DEFAULT_CURRENCY{"EUR"};
} // namespace MO
