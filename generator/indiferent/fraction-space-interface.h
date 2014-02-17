/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTION_SPACE_INTERFACE_H_INCLUDED
#define FRACTION_SPACE_INTERFACE_H_INCLUDED

class Axis;
class IFractionCell;

class IFractionSpace
{
public:
    virtual void initQuantities() = 0;
    virtual void calculateSourceEvolution(double dt) = 0;
    virtual void calculateFlowsEvolution(double dt) = 0;
    virtual void swapBuffers() = 0;
    
    virtual IFractionCell* getCell(const double* coords) = 0;
    
    virtual double getQuantitiesSum(unsigned int quantityIndex) = 0;
    
    virtual const Axis* getAxisDescription(unsigned int axis) = 0;
    
    virtual ~IFractionSpace() {}
};

#endif // FRACTION_SPACE_INTERFACE_H_INCLUDED

