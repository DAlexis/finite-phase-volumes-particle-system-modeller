#ifndef FRACTION1_H_INCLUDED
#define FRACTION1_H_INCLUDED

#include "global-defines.h"
#include "fraction-base.h"

class Fraction1Cell;

typedef FractionCellBase<FRACTION_FRACTION1,
                     SPACE_COORDS_COUNT,
                     FRACTION1_COORDS_COUNT,
                     FRACTION1_QUANTITIES_COUNT,
                     Fraction1Cell>       Fraction1CellBase;

typedef FractionSpaceBase<FRACTION1_COORDS_COUNT, Fraction1Cell> Fraction1SpaceBase;

class Fraction1Cell : public Fraction1CellBase
{
public:
    virtual ~Fraction1Cell() { }
    virtual void calculateDerivatives();
    virtual void calculateSourceEvolution(double dt);
    
private:
    void* getModel();
};

class Fraction1Space : public Fraction1SpaceBase
{
public:
    Fraction1Space(FractionsPool* parentFractionsPool);
    virtual ~Fraction1Space() { }
    
private:
};

#endif // FRACTION1_H_INCLUDED
