#include "model.h"

Model::Model() :
    space(this),
    outputMaker(&space),
    time(0)
{
    outputMaker.addInstance(new Fraction1ConcentrationVsCoordsOutput);
    outputMaker.addInstance(new Fraction1ConcentrationVsVelocityOutput);
}

void Model::iterate(double dt)
{
    outputMaker.output(time);
    space.calculateFlowsEvolution(dt);
    space.calculateSourceEvolution(dt);
    space.swapBuffers();
    time += dt;
}
