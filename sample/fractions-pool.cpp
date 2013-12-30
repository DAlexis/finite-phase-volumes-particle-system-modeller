#include "model.h"
#include "fractions-pool.h"

FractionsPool::FractionsPool()
{
    initFractionsPoolBase();
}

FractionsPool::~FractionsPool()
{
}

void FractionsPool::createFractions()
{
    fractions[FRACTION_FRACTION1] = new Fraction1Space(this);
}
