/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef GLOBAL_DEFINES_H_INCLUDED
#define GLOBAL_DEFINES_H_INCLUDED

#include <math.h>

#define EVERY_FRACTION_COUNT_QUANTITY_INDEX     0

//#define DOUBLE_NULL        1e-8

typedef unsigned int uint;

#include "grid-template.h"

inline bool isNotNull(double x)
{
    return (x != 0.0);
    /*
    if (fabs(x) < DOUBLE_NULL) return false;
    return true;*/
}

inline bool isNull(double x)
{
    return (x == 0.0);
    /*
    if (fabs(x) < DOUBLE_NULL) return true;
    return false;
    */
}

inline bool isNotCount(double x)
{
    return (x <= 0.0);
    /*
    if (x < DOUBLE_NULL) return true;
    return false;*/
}

//#define USE_NATIVE_LOCKS
//#define USE_NATIVE_UNLOCK

#ifdef USE_NATIVE_LOCKS
    #define USE_NATIVE_UNLOCK
#endif

#endif // GLOBAL_DEFINES_H_INCLUDED
