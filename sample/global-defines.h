/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef GLOBAL_DEFINES_H_INCLUDED
#define GLOBAL_DEFINES_H_INCLUDED

#include <math.h>

#define EVERY_FRACTION_COUNT_QUANTITY_INDEX     0

#define DOUBLE_NULL        1e-8

typedef unsigned int uint;

class FractionsPool;

#include "grid-template.h"

#include "model-defines.h"

// Grid instance type for coordinate space
typedef Grid<SPACE_COORDS_COUNT, FractionsPool> SpaceGridType;

inline bool isNotNull(double x)
{
    if (fabs(x) < DOUBLE_NULL) return false;
        return true;
}



#endif // GLOBAL_DEFINES_H_INCLUDED
