/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTIONS_POOL_TEMPLATE
#define FRACTIONS_POOL_TEMPLATE

#include "global-defines.h"
#include "interfaces.h"

template<int SpaceDimension, int FractionsCount>
class FractionsPoolBase : public GridElementBase<SpaceDimension>, public IFractionsPool
{
public:
    FractionsPoolBase()
    {
        for (unsigned int i=0; i<FractionsCount; i++)
            fractions[i] = NULL;
    }
    
    virtual ~FractionsPoolBase()
    {
        for (unsigned int i=0; i<FractionsCount; i++)
            if (fractions[i]) delete fractions[i];
    }
    
    void initQuantities()
    {
        for (unsigned int i=0; i<FractionsCount; i++)
            fractions[i]->initQuantities();
    }

    void calculateSourceEvolution(double dt)
    {
        for (unsigned int i=0; i<FractionsCount; i++)
            fractions[i]->calculateSourceEvolution(dt);
    }

    void calculateFlowsEvolution(double dt)
    {
        for (unsigned int i=0; i<FractionsCount; i++)
            fractions[i]->calculateFlowsEvolution(dt);
    }
    
    void swapBuffers()
    {
        for (unsigned int i=0; i<FractionsCount; i++)
            fractions[i]->swapBuffers();
    }
    
    virtual void createFractions() = 0;

    IFractionSpace *fractions[FractionsCount];

protected:
    void initFractionsPoolBase() { createFractions(); }
};

//class FractionsPoolBase : public FractionsPoolBaseTmp<SPACE_COORDS_COUNT> {};

#endif // FRACTIONS_POOL_TEMPLATE
