#include "model.h"

Model::Model() :
    space(this),
    outputMaker(&space),
    time(0)
{
    OutputInstance* instance1 = new OutputInstance;
    instance1->getFractionPoint()[0] = 0;
    instance1->getSpacePoint()[0] = 0;
    
    instance1
        ->setFilenamePrefix("Output-instance-1")
        ->setFractionAndQuantity(FRACTION_FRACTION1, FRACTION1_QUANTITIES_PARTICLES_COUNT)
        ->configAxis(0, OAT_SPACE_COORDINATE, 100, SPACE_COORDS_X_COORD)
        ->setPeriod(0.04);
    
    
    OutputInstance* instance2 = new OutputInstance;
    instance2->getFractionPoint()[0] = 0;
    instance2->getSpacePoint()[0] = 0;
    
    instance2
        ->setFilenamePrefix("Output-instance-2")
        ->setFractionAndQuantity(FRACTION_FRACTION1, FRACTION1_QUANTITIES_PARTICLES_COUNT)
        ->configAxis(0, OAT_FRACTION_COORDINATE, 100, FRACTION1_COORDS_X_SPEED)
        ->setPeriod(0.04);
    
    OutputInstance* instance3 = new OutputInstance;
    instance3->getFractionPoint()[0] = 0;
    instance3->getSpacePoint()[0] = 0;
    
    instance3
        ->configAxis(0, OAT_SPACE_COORDINATE, 100, SPACE_COORDS_X_COORD)
        ->configAxis(1, OAT_FRACTION_COORDINATE, 100, FRACTION1_COORDS_X_SPEED)
        ->setFractionAndQuantity(FRACTION_FRACTION1, FRACTION1_QUANTITIES_PARTICLES_COUNT)
        ->setFilenamePrefix("Output-instance-3")
        ->setPeriod(0.5);
    
    outputMaker.addInstance(instance1);
    outputMaker.addInstance(instance2);
    outputMaker.addInstance(instance3);
/*    outputMaker.addInstance(new Fraction1ConcentrationVsCoordsOutput);
    outputMaker.addInstance(new Fraction1ConcentrationVsVelocityOutput);*/
}

void Model::iterate(double dt)
{
    outputMaker.output(time);
    space.calculateFlowsEvolution(dt);
    space.calculateSourceEvolution(dt);
    space.swapBuffers();
    time += dt;
}
