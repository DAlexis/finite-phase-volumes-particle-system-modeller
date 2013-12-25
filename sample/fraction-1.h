#ifndef FRACTION_1_H_INCLUDED
#define FRACTION_1_H_INCLUDED

#include "grid-template.h"
#include "fraction-cell-template.h"
#include "fraction-space-template.h"
// This fraction cell, this fraction space and fractions pool to use pointer to it 

#include "global-defines.h"

typedef FractionCell<FRACTION_FRACTION1,
                     SPACE_COORDS_COUNT,
                     FRACTION1_COORDS_COUNT,
                     FRACTION1_QUANTITIES_COUNT,
                     Fraction1SpaceGridType>       Fraction1CellBase;

class Fraction1Cell : public Fraction1CellBase
{
public:
    /// Fraction-specific calculations
    void calculateDerivatives();
    
    void calculateSourceEvolution(double dt);
    void calculateFlowsEvolution(double dt);
    /// Fraction-specific initialisation
    Fraction1Cell();
private:
    
    void* getModel();
    
    double getFlowInSpace(unsigned int coordinate, unsigned int quantity, TransferDirection direction, Fraction1Cell* neighbor);
    double getFlowInFractionSpace(unsigned int coordinate, unsigned int quantity, TransferDirection direction, Fraction1Cell* neighbor);
};

class Fraction1Space : public FractionSpace<Fraction1SpaceGridType>
{
public:
    /// Setting up axis
    Fraction1Space(FractionsPool* parentFractionsPool);
    
    /// @todo Move this functions to template
    void calculateFlowsEvolution(double dt);
    void calculateSourceEvolution(double dt);
    
private:
    
};

#endif // FRACTION_1_H_INCLUDED
