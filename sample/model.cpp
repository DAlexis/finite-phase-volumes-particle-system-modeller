#include "model.h"

Model::Model() :
    outputMaker(&space),
    time(0)
{
    outputMaker.addInstance(new Fraction1ConcentrationOutput);
}

void Model::iterate(double dt)
{
    outputMaker.output(time);
    space.calculateFlowsEvolution(dt);
    space.calculateSourceEvolution(dt);
    space.swapBuffers();
    time += dt;
}
