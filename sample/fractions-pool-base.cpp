/*
 * This file is model-independent and should be used as is in any generated model
 */

#include "fractions-pool-base.h"

FractionsPoolBase::FractionsPoolBase()
{
    for (unsigned int i=0; i<FRACTIONS_COUNT; i++)
        fractions[i] = NULL;
}

FractionsPoolBase::~FractionsPoolBase()
{
    for (unsigned int i=0; i<FRACTIONS_COUNT; i++)
        if (fractions[i]) delete fractions[i];
}

void FractionsPoolBase::initFractionsPoolBase()
{
    createFractions();
}

void FractionsPoolBase::calculateSourceEvolution(double dt)
{
    for (unsigned int i=0; i<FRACTIONS_COUNT; i++)
        fractions[i]->calculateSourceEvolution(dt);
}

void FractionsPoolBase::calculateFlowsEvolution(double dt)
{
    for (unsigned int i=0; i<FRACTIONS_COUNT; i++)
        fractions[i]->calculateFlowsEvolution(dt);
}

void FractionsPoolBase::swapBuffers()
{
    for (unsigned int i=0; i<FRACTIONS_COUNT; i++)
        fractions[i]->swapBuffers();
}
