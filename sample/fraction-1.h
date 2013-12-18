#ifndef FRACTION_1_H_INCLUDED
#define FRACTION_1_H_INCLUDED

#include "grid-template.h"
#include "fraction-cell-template.h"
#include "fraction-space-template.h"
// This fraction cell, this fraction space and fractions pool to use pointer to it 

class Fraction1Cell;
class Fraction1Space;
class FractionsPool;
class Space;

typedef Grid<FRACTION1_COORDS_COUNT, Fraction1Cell> Fraction1SpaceGridType;

class Fraction1Cell : public FractionCell<SPACE_COORDS_COUNT, FRACTION1_COORDS_COUNT, FRACTION1_QUANTITIES_COUNT, Fraction1SpaceGridType>
{
public:
    /// Fraction-specific calculations
    void calculateDerivatives();
    
    /// Fraction-specific initialisation
    Fraction1Cell();
private:
    
    void* getSpaceCell();
    void* getFractionPool();
};

class Fraction1Space : public FractionSpace<Fraction1SpaceGridType>
{
public:
    /// Setting up axis
    Fraction1Space(FractionsPool* parentFractionsPool);
    
    void calculateFlowsEvolution(double dt);

private:
    
};

#endif // FRACTION_1_H_INCLUDED
