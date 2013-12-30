#ifndef FRACTION_1_H_INCLUDED
#define FRACTION_1_H_INCLUDED

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


/*

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
*/
#endif // FRACTION_1_H_INCLUDED
