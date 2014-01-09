#include "space.h"

#include "fractions-pool.h"

Space::Space(void* model) :
    parent(model)
{
    Axis& x_coord = spaceGridDescription.axis[SPACE_COORDS_X_COORD];
    x_coord.uniformInit(-10.0, 10.0, 100);
    x_coord.setName("Horisontal coordinate, m");
    
    
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

