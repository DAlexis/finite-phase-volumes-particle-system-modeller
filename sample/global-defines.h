#ifndef GLOBAL_DEFINES_H_INCLUDED
#define GLOBAL_DEFINES_H_INCLUDED

#define DOUBLE_NULL        1e-8

#include <math.h>

typedef unsigned int uint;

inline bool isNotNull(double x)
{
    if (fabs(x) < DOUBLE_NULL) return false;
        return true;
}


#endif // GLOBAL_DEFINES_H_INCLUDED
