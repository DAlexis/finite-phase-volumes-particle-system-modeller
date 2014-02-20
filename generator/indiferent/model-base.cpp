/*
 * This file is model-independent and should be used as is in any generated model
 */
 
#include "model-base.h"

ModelBase::ModelBase() :
    space(this),
    outputMaker(&space),
    time(0)
{
    space.initQuantities();
}

void ModelBase::iterate(double dt)
{
    outputMaker.output(time);
    space.calculateEvolution(dt);
    space.swapBuffers();
    time += dt;
}
