#include "fraction-1.h"
#include "model.h"

Fraction1Space::Fraction1Space(FractionsPool* parentFractionsPool) :
    Fraction1SpaceBase(parentFractionsPool)
{
    Axis& x_speed = fractionGridDescription.axis[FRACTION1_COORDS_X_SPEED];
    x_speed.uniformInit(-10.0, 10.0, 50);
    x_speed.setName("Horisontal speed, m/s");
    
    
    constructGrid(fractionGridDescription);
}

void Fraction1Cell::calculateDerivatives()
{
    spaceCoordsDerivatives[SPACE_COORDS_X_COORD] = this->coordinates[FRACTION1_COORDS_X_SPEED];

    fractionCoordsDerivatives[FRACTION1_COORDS_X_SPEED] = 0;// -9.8;

}

void Fraction1Cell::calculateSourceEvolution(double dt)
{
    SpaceGridType::GridElementBase* spaceCell = getSpaceCell();
    // Generating initial state of particles
    if (static_cast<Model*>(getModel())->time < 0.02)
    if (fabs(coordinates[FRACTION1_COORDS_X_SPEED]+3) < 0.5)
    if (fabs(spaceCell->coordinates[SPACE_COORDS_X_COORD]-8) < 1)
    //    nextStepQuantities[FRACTION1_QUANTITY_COUNT] += 0.1*spaceCell->volume*dt;
    nextStepQuantities[FRACTION1_QUANTITIES_PARTICLES_COUNT] = 1;//spaceCell->coordinates[SPACE_COORDS_X]/30;

}

void* Fraction1Cell::getModel()
{
    return static_cast<Space*>(getSpaceCell()->parentGrid)->parent;
}
