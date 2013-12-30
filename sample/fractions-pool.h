#ifndef FRACTIONS_POOL_H_INCLUDED
#define FRACTIONS_POOL_H_INCLUDED

#include "fraction-1.h"
#include "fractions-pool-base.h"

class FractionsPool : public FractionsPoolBase
{
public:
    FractionsPool();
    virtual ~FractionsPool();
    virtual void createFractions();
};

#endif // FRACTIONS_POOL_H_INCLUDED
