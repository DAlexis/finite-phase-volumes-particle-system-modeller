#include "space.h"

Space::Space()
{
    // Setting up x coordinate
    Axis& xCoord = spaceGridDescription.axis[0];
    xCoord.uniformInit(-10.0, 10.0, 20);
    xCoord.setName("Horisontal coordinate, m");
    
    constructGrid(spaceGridDescription);
}
