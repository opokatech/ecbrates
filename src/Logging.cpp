#include "Logging.hpp"

#ifdef DEBUG
#include <cstdarg>
#include <cstdio>
#endif

namespace MO
{
    void Log(const char *a_format, ...)
    {
#ifdef DEBUG
        va_list args;
        va_start(args, a_format);
        vprintf(a_format, args);
        va_end(args);
#else
        (void)a_format; // don't warn about unused variable
#endif
    }

} // namespace MO
