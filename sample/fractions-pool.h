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

/*
class FractionsPool : public FractionsPoolTemplate
{
public:
    Fraction1Space fraction1;
    FractionsPool();
    virtual ~FractionsPool() {}
    
    void calculateFlowsEvolution(double dt);
    void calculateSourceEvolution(double dt);
    void swapBuffers();
    
private:

};
*/
#endif // FRACTIONS_POOL_H_INCLUDED
