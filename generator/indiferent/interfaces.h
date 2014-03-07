/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTION_CELL_INTERFACE_H_INCLUDED
#define FRACTION_CELL_INTERFACE_H_INCLUDED

#include <vector>

class Axis;

class IFractionCell
{
public:
    virtual void initQuantities() = 0;
    virtual void calculateDerivatives() = 0;
    virtual void calculateSourceEvolution(double dt) = 0;
    virtual void calculateFlowsEvolution(double dt) = 0;
    
    /** @brief Calculate diffusion for all quantities except particles count 
     *          in positive axis direction from this cell to next. 
     *          This and next cells are changed.
     */
    virtual void calculateQuantitiesDiffusion(double dt) = 0;
    virtual void calculateIntensiveQuantities() = 0;
    virtual void addDelta() = 0;
    
    virtual double getQuantitiesDensity(unsigned int index) = 0;
    virtual double getQuantitiesDensityConvolution(unsigned int quantityIndex, const std::vector<unsigned int>& convoluteBy) = 0;
    virtual double getIntensiveQuantity(unsigned int intensiveQuantityIndex) = 0;
    
    virtual ~IFractionCell() {}
};

class IFractionSpace
{
public:
    virtual void initQuantities() = 0;
    virtual void calculateSourceEvolution(double dt) = 0;
    virtual void calculateFlowsEvolution(double dt) = 0;
    virtual void addDelta() = 0;
    
    virtual IFractionCell* getCell(const double* coords) = 0;
    
    virtual double getQuantitiesSum(unsigned int quantityIndex) = 0;
    
    virtual const Axis* getAxisDescription(unsigned int axis) = 0;
    
    virtual ~IFractionSpace() {}
};

class IFractionsPool
{
public:
    virtual IFractionSpace* getFraction(unsigned int fractionId) = 0;
    virtual double getVolume() = 0;
};

class ISpace
{
public:
    virtual IFractionsPool* getCell_d(const double* coords) = 0;
    virtual IFractionsPool* getCell_ui(const unsigned int* coords) = 0;
    virtual const Axis* getAxisDescription(unsigned int axis) = 0;
};

#endif // FRACTION_CELL_INTERFACE_H_INCLUDED
