#include "fractions-pool.h"

FractionsPool::FractionsPool() :
    fraction1(this)
{
    fractions[0] = &fraction1;
}

void FractionsPool::calculateFlowsEvolution(double dt)
{
    fraction1.calculateFlowsEvolution(dt);
}

void FractionsPool::calculateSourceEvolution(double dt)
{
    fraction1.calculateSourceEvolution(dt);
}

void FractionsPool::swapBuffers()
{
    fraction1.swapBuffers();
}
