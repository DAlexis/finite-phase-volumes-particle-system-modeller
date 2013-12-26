#include "model.h"

Model::Model() :
    space(this),
    outputMaker(&space),
    time(0)
{/*
    outputMaker.addInstance(new Fraction1ConcentrationVsCoordsOutput);
    outputMaker.addInstance(new Fraction1ConcentrationVsVelocityOutput);
    */
    double spacePoint[SPACE_COORDS_COUNT] = { 0 };
    
    FractionsPool* spaceCell = static_cast<FractionsPool*>(&(space.elements[0]));
    Fraction1Space* fr1 = &(spaceCell->fraction1);
    Fraction1SpaceGridType* sp1 = static_cast<Fraction1SpaceGridType*>(fr1); 
    
    Fraction1Cell* cell = static_cast<Fraction1Cell*>( &(spaceCell->fraction1.elements[0]) );
    double result = cell->quantities[FRACTION1_QUANTITY_COUNT];
}

void Model::iterate(double dt)
{
    outputMaker.output(time);
    space.calculateFlowsEvolution(dt);
    space.calculateSourceEvolution(dt);
    space.swapBuffers();
    time += dt;
}
