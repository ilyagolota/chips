#include <cocos2d.h>
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#   include <sys\timeb.h>
#else
#   include <time.h>
#endif

#include "Date.h"

using namespace utils;

double Date::now()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    timeb buffer;
    ftime(&buffer);
    return (double)buffer.time + buffer.millitm * 0.001;
#else
    timeval buffer;
    gettimeofday(&buffer, NULL);
    return (double)buffer.tv_sec + buffer.tv_usec * 0.000001;
#endif
}
