#ifndef FRACTION_1_H_INCLUDED
#define FRACTION_1_H_INCLUDED

#include "grid-template.h"

// This fraction cell, this fraction space and fractions pool to use pointer to it 
class FractionsPool;
class Fraction1Space;
class Fraction1Cell;

typedef Grid<1, Fraction1Cell> Fraction1GridType;

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
    double xDer;
    
    /// Fraction coordinates derivatives
    double vxDer;
    
    void calculateDerivatives();
    
    Fraction1Cell();
    
    void init(Fraction1GridType::GridElement* parentGridElement);
    
private:
    Fraction1GridType::GridElement* parent;
};

class Fraction1Space : public Fraction1GridType
{
public:
    Fraction1Space(FractionsPool* parentFractionsPool);
    
private:
    FractionsPool* parent;
    Fraction1GridType::GridDescription gridDescription;
};

#endif // FRACTION_1_H_INCLUDED
