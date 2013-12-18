#ifndef GLOBAL_DEFINES_H_INCLUDED
#define GLOBAL_DEFINES_H_INCLUDED

#include <math.h>

/// @todo Structurise this file. Separate model-specific defines

typedef unsigned int uint;

#define DOUBLE_NULL        1e-8

//////////////////////////
// Space enums
enum SpaceCoordinate {
    SPACE_COORDS_X = 0,
    SPACE_COORDS_COUNT
};

enum Fraction
{
    FR_FRACTION1 = 0,
    FR_COUNT
};
    

//////////////////////////
// Fraction 1 enums
enum Fraction1Quantity
{
    FRACTION1_QUANTITY_CONCENTRATION = 0,
    FRACTION1_QUANTITIES_COUNT
};

enum Fraction1Coordinate
{
    FRACTION1_COORDS_VX = 0,
    FRACTION1_COORDS_COUNT
};

inline bool isNotNull(double x)
{
    if (fabs(x) < DOUBLE_NULL) return false;
        return true;
}


#endif // GLOBAL_DEFINES_H_INCLUDED
