/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTION_SPACE_INTERFACE_H_INCLUDED
#define FRACTION_SPACE_INTERFACE_H_INCLUDED

class IFractionSpace
{
public:
    virtual void calculateSourceEvolution(double dt) = 0;
    virtual void calculateFlowsEvolution(double dt) = 0;
    virtual void swapBuffers() = 0;
    
    virtual ~IFractionSpace() {}
};

#endif // FRACTION_SPACE_INTERFACE_H_INCLUDED

