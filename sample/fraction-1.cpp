#include "fraction-1.h"

Fraction1Space::Fraction1Space()
{
    // Setting up x coordinate
    Axis& xSpeed = gridDescription.axis[0];
    xSpeed.uniformInit(-100.0, 100.0, 10);
    xSpeed.setName("Horisontal speed, m");
    
    constructGrid(gridDescription);
}
