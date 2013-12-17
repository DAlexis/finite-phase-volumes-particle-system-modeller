#include "fraction-1.h"

Fraction1Cell::Fraction1Cell()
{
    
}

void Fraction1Cell::init(Fraction1GridType::GridElement* parentGridElement)
{
    parent = parentGridElement;
}

void Fraction1Cell::calculateDerivatives()
{
    vxDer = -9.8;
    xDer = 0;
}

Fraction1Space::Fraction1Space(FractionsPool* parentFractionsPool) :
    parent(parentFractionsPool)
{
    // Setting up x coordinate
    Axis& xSpeed = gridDescription.axis[0];
    xSpeed.uniformInit(-100.0, 100.0, 10);
    xSpeed.setName("Horisontal speed, m");
    
    constructGrid(gridDescription);
}
