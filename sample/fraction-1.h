#ifndef FRACTION_1_H_INCLUDED
#define FRACTION_1_H_INCLUDED

#include "grid-template.h"

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
    
private:
    
};

class Fraction1Space : public Grid<1, Fraction1Cell>
{
public:
    void init(const GridElement<1, Fraction1Space>* parentCell);
    
    Fraction1Space();
    
private:
    /// Space grid element, where this fraction space located
    GridElement<1, Fraction1Space> *parent;
    
    GridDescription<1> gridDescription;
};

#endif // FRACTION_1_H_INCLUDED
