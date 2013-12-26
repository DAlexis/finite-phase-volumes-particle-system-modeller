/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTIONS_POOL_TEMPLATE
#define FRACTIONS_POOL_TEMPLATE

#include "global-defines.h"
#include "fraction-cell-interface.h"

class FractionsPoolBase : public SpaceGridType::GridElement
{
public:
    FractionsPoolBase();
    virtual ~FractionsPoolBase();
    
    void calculateSourceEvolution(double dt);
    void calculateFlowsEvolution(double dt);
    void swapBuffers();
    
    virtual void createFractions() = 0;

    void *fractions[FRACTIONS_COUNT];

protected:
    void initFractionsPoolBase();
};

#endif // FRACTIONS_POOL_TEMPLATE
