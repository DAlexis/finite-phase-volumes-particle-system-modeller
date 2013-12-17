#include "fraction-1.h"
#include "space.h"

Fraction1Cell::Fraction1Cell()
{
    
}

void* Fraction1Cell::getSpaceCell()
{
    /// @todo Remove this temporary pointers and write 1-line optimal code
    FractionsPool* pFractionsPool = static_cast<FractionsPool*>(getFractionPool());
    SpaceGridType::GridElement* pSpaceGridElement = static_cast<SpaceGridType::GridElement*>(pFractionsPool->parent);
    return pSpaceGridElement;
}

void* Fraction1Cell::getFractionPool()
{
    /// @todo Remove this temporary pointers and write 1-line optimal code
    Fraction1GridType* pFractionGrid = static_cast<Fraction1GridType*>(parent->parentGrid);
    Fraction1Space* pFractionSpace = static_cast<Fraction1Space*>(pFractionGrid);
    FractionsPool* pFractionsPool = pFractionSpace->parent;
    return pFractionsPool;
}

void Fraction1Cell::init(Fraction1GridType::GridElement* parentGridElement)
{
    parent = parentGridElement;
}

void Fraction1Cell::calculateDerivatives()
{
    spaceCoordsDerivatives[Space::SPACE_COORDS_X] = parent->coordinates[FRACTION1_COORDS_VX];
    fractionCoordsDerivatives[FRACTION1_COORDS_VX] = -9.8;
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
