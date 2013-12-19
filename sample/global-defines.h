/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef GLOBAL_DEFINES_H_INCLUDED
#define GLOBAL_DEFINES_H_INCLUDED

#include "model-defines.h"
#include <math.h>

#define DOUBLE_NULL        1e-8

typedef unsigned int uint;

inline bool isNotNull(double x)
{
    if (fabs(x) < DOUBLE_NULL) return false;
        return true;
}

#endif // GLOBAL_DEFINES_H_INCLUDED
