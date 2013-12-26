/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTIONS_POOL_TEMPLATE
#define FRACTIONS_POOL_TEMPLATE

#include "global-defines.h"

class FractionsPoolTemplate : public SpaceGridType::GridElement
{
public:
    virtual ~FractionsPoolTemplate() {}
    
    // To get fraction by index
    void* fractions[FRACTIONS_COUNT];
};

#endif // FRACTIONS_POOL_TEMPLATE
