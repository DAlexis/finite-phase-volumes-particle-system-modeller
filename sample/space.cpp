#include "space.h"

SpaceGrid::SpaceGrid()
{
    // Setting up x coordinate
    Axis& xCoord = gridDescription.axis[0];
    xCoord.uniformInit(-10.0, 10.0, 10);
    xCoord.setName("Horisontal coordinate, m");
    
    constructGrid(gridDescription);
}
