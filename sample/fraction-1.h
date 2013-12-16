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
    
    /// Fraction variables
    
    
private:

};

class Fraction1Space : public Grid<1, Fraction1Cell>
{
public:
    void init(const GridElement<1, Fraction1Space>* parentCell);
    
private:
    /// Space grid element, where this fraction space located
    GridElement<1, Fraction1Space> *parent;
};



class Fraction2
{

};

#endif // FRACTION_1_H_INCLUDED
