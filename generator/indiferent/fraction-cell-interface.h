/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTION_CELL_INTERFACE_H_INCLUDED
#define FRACTION_CELL_INTERFACE_H_INCLUDED

#include <vector>

class IFractionCell
{
public:
    virtual void calculateDerivatives() = 0;
    virtual void calculateSourceEvolution(double dt) = 0;
    virtual void calculateFlowsEvolution(double dt) = 0;
    virtual void calculateSecondaryQuantities() = 0;
    virtual void swapBuffers() = 0;
    
    virtual double getQuantitiesDensity(unsigned int index) = 0;
    virtual double getQuantitiesDensityConvolution(unsigned int quantityIndex, const std::vector<unsigned int>& convoluteBy) = 0;
    
    virtual ~IFractionCell() {}
};

#endif // FRACTION_CELL_INTERFACE_H_INCLUDED
