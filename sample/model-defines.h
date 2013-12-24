#ifndef MODEL_DEFINES_H_INCLUDED
#define MODEL_DEFINES_H_INCLUDED

#include "grid-template.h"

//////////////////////////
// General
enum Fraction
{
    FRACTION_FRACTION1 = 0,
    FRACTIONS_COUNT
};

//////////////////////////
// Space defines
enum SpaceCoordinate {
    SPACE_COORDS_X = 0,
    SPACE_COORDS_COUNT
};

//////////////////////////
// Fraction 1 defines
enum Fraction1Coordinate
{
    FRACTION1_COORDS_VX = 0,
    FRACTION1_COORDS_COUNT
};

enum Fraction1Quantity
{
    FRACTION1_QUANTITY_COUNT = 0,
    FRACTION1_QUANTITIES_COUNT
};

class Fraction1Cell;
class Fraction1Space;

typedef Grid<FRACTION1_COORDS_COUNT, Fraction1Cell> Fraction1SpaceGridType;

#endif // MODEL_DEFINES_H_INCLUDED
