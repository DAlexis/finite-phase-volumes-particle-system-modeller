/*
 * This file is model-independent and should be used as is in any generated model
 */
 
#include "model-base.h"

#include <math.h>
#include <iostream>
#include <iomanip>

ModelBase::ModelBase() :
    outputMaker(&space),
    time(0)
{
    space.setParent(this);
    space.initQuantities();
}

ModelBase::~ModelBase()
{
    space.stopThreads();
}

void ModelBase::iterate(double dt)
{
    outputMaker.output(time);
    space.calculateEvolution(dt);
    space.swapBuffers();
    time += dt;
}

void ModelBase::setThreadsCount(unsigned int count)
{
    space.initThreads(count);
}

void ModelBase::run(double stopTime, double timeStep)
{
    int lastProgress = -10;
    while (time < stopTime)
    {
        iterate(timeStep);
        double progress = floor(time / stopTime * 100);
        if ((progress - lastProgress) >= 1)
        {
            lastProgress = progress;
            std::cout << '\r' << std::setw ( 4 ) << progress << "  %" << std::flush;
        }
    }
}
