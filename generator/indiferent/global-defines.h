/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef GLOBAL_DEFINES_H_INCLUDED
#define GLOBAL_DEFINES_H_INCLUDED

#include <math.h>

#define EVERY_FRACTION_COUNT_QUANTITY_INDEX     0

#define DOUBLE_NULL        1e-8

typedef unsigned int uint;

#include "grid-template.h"

inline bool isNotNull(double x)
{
    if (fabs(x) < DOUBLE_NULL) return false;
    return true;
}

inline bool isNull(double x)
{
    if (fabs(x) < DOUBLE_NULL) return true;
    return false;
}

inline bool isNotCount(double x)
{
    if (x < DOUBLE_NULL) return true;
    return false;
}

#define THREADS_COUNT       3

//#define USE_NATIVE_LOCKS

#endif // GLOBAL_DEFINES_H_INCLUDED
