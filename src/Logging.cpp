#include "Logging.hpp"

#ifndef NDEBUG
    #include <cstdarg>
    #include <cstdio>
#endif

namespace ECB
{
    void Log(const char *a_format, ...)
    {
#ifndef NDEBUG
        va_list args;
        va_start(args, a_format);
        vprintf(a_format, args);
        va_end(args);
#else
        (void)a_format; // don't warn about unused variable
#endif
    }

} // namespace ECB
