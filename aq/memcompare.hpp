#pragma once


#include <cstring>  // for gnu specific `memmem` function.

#ifndef _GNU_SOURCE

// replace line below with `memmem` implementation when you need it !!
#error must implement `memmem` for this platform 
#endif

// `memmem` is GNU specific, so would need to be implemented manually for any other platform.

namespace aq
{
    template<typename T>
    inline T* memmem(T* hackstack, size_t haystacklen, T* needle, size_t needlelen)
    {
        return reinterpret_cast<T*>(memmem(hackstack, haystacklen, needle, needlelen));
    }

    template<typename T>
    inline T* memchr(T* s, int c, size_t n)
    {
        return reinterpret_cast<T*>(memchr(s, c, n));
    }

}