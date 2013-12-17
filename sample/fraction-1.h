#ifndef FRACTION_1_H_INCLUDED
#define FRACTION_1_H_INCLUDED

#include "grid-template.h"

// This fraction cell, this fraction space and fractions pool to use pointer to it 

class Fraction1Cell;
class Fraction1Space;
class FractionsPool;
class Space;

#define FRACTION1_SPACE_DIMENSION   1

typedef Grid<FRACTION1_SPACE_DIMENSION, Fraction1Cell> Fraction1GridType;

class Fraction1Cell
{
public:
    enum Variables
    {
        FRACTION1_VARS_CONCENTRATION = 0,
        FRACTION1_VARS_COUNT
    };
    
    enum Coordinates
    {
        FRACTION1_COORDS_VX = 0,
        FRACTION1_COORDS_COUNT
    };
    
    /// Space coordinates derivatives
    double spaceCoordsDerivatives[SPACE_DIMENSION];
    
    /// Fraction coordinates derivatives
    double fractionCoordsDerivatives[FRACTION1_SPACE_DIMENSION];
    
    void calculateDerivatives();
    
    Fraction1Cell();
    
    void init(Fraction1GridType::GridElement* parentGridElement);
    
private:
    Fraction1GridType::GridElement* parent;
    
    void* getSpaceCell();
    void* getFractionPool();
};

class Fraction1Space : public Fraction1GridType
{
public:
    Fraction1Space(FractionsPool* parentFractionsPool);
    FractionsPool* parent;
    
private:
    
    Fraction1GridType::GridDescription gridDescription;
};

#endif // FRACTION_1_H_INCLUDED
