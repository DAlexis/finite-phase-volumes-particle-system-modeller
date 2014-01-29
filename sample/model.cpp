#include "model.h"

Model::Model() :
    space(this),
    outputMaker(&space),
    time(0)
{
    OutputInstance* instance1 = new OutputInstance(&outputMaker);
    instance1->setFilenamePrefix("Output-instance-1");
    instance1->getFractionPoint()[0] = 0;
    instance1->getSpacePoint()[0] = 0;
    instance1->setFractionAndQuantity(FRACTION_FRACTION1, FRACTION1_QUANTITIES_PARTICLES_COUNT);
    instance1->configAxis(0,
        OAT_SPACE_COORDINATE,
        100,
        SPACE_COORDS_X_COORD);
    instance1->setPeriod(0.04);
    outputMaker.addInstance(instance1);
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
