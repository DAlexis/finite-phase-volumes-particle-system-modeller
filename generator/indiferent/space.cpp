#include "space.h"

#include "fractions-pool.h"

Space::Space(void* model) :
    parent(model)
{
    // Setting up x coordinate
    Axis& xCoord = spaceGridDescription.axis[0];
    xCoord.uniformInit(-10.0, 10.0, 50);
    xCoord.setName("Horisontal coordinate, m");
    
    constructGrid(spaceGridDescription);
}

void Space::calculateFlowsEvolution(double dt)
{
    for (size_t i=0; i<elementsCount; i++)
         elements[i].calculateFlowsEvolution(dt);
}

void Space::calculateSourceEvolution(double dt)
{
    for (size_t i=0; i<elementsCount; i++)
        elements[i].calculateSourceEvolution(dt);
}

void Space::swapBuffers()
{
    for (size_t i=0; i<elementsCount; i++)
        elements[i].swapBuffers();
}

