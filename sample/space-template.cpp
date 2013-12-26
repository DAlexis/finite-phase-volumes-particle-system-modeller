#include "space-template.h"

void SpaceBase::calculateFlowsEvolution(double dt)
{
    for (size_t i=0; i<elementsCount; i++)
        static_cast<FractionsPoolBase*> ( &(elements[i]) )->calculateFlowsEvolution(dt);
}

void SpaceBase::calculateSourceEvolution(double dt)
{
    for (size_t i=0; i<elementsCount; i++)
        static_cast<FractionsPoolBase*> ( &(elements[i]) )->calculateSourceEvolution(dt);
}

void SpaceBase::swapBuffers()
{
    for (size_t i=0; i<elementsCount; i++)
        static_cast<FractionsPoolBase*> ( &(elements[i]) )->swapBuffers();
}
