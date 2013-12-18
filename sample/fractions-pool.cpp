#include "fractions-pool.h"

FractionsPool::FractionsPool() :
    fraction1(this)
{
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
