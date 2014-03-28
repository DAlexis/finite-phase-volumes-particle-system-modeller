/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTIONS_POOL_TEMPLATE
#define FRACTIONS_POOL_TEMPLATE

#include "global-defines.h"
#include "interfaces.h"
#include <string.h>
#include <string>
#include <vector>

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

    void calculateEvolution(double dt)
    {
        for (unsigned int i=0; i<FractionsCount; i++) {
            fractions[i]->calculateSourceEvolution(dt);
            fractions[i]->calculateFlowsEvolution(dt);
        }
    }
    
    void addDelta()
    {
        for (unsigned int i=0; i<FractionsCount; i++)
            fractions[i]->addDelta();
    }
    
    void averageWithNeighbours(unsigned int fractionId)
    {
        fractions[fractionId]->averageWithNeighbours();
    }
    
    IFractionSpace* getFraction(unsigned int fractionId) { return fractions[fractionId]; }
    double getVolume() { return this->volume; }
    
    IFractionSpace *fractions[FractionsCount];
    
};


#endif // FRACTIONS_POOL_TEMPLATE
