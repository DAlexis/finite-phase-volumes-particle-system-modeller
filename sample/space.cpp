#include "space.h"

Space::Space(void* model) :
    SpaceBase(model)
{
    // Setting up x coordinate
    Axis& xCoord = spaceGridDescription.axis[0];
    xCoord.uniformInit(-10.0, 10.0, 2);
    xCoord.setName("Horisontal coordinate, m");
    
    constructGrid(spaceGridDescription);
}
