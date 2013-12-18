#include "model.h"

void Model::iterate(double dt)
{
    space.calculateFlowsEvolution(dt);
    space.calculateSourceEvolution(dt);
    space.swapBuffers();
}
