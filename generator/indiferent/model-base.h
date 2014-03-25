/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef MODEL_BASE_H_INCLUDED
#define MODEL_BASE_H_INCLUDED

#include "space-base.h"
#include "output.h"

#include <math.h>
#include <iostream>
#include <iomanip>

template <class SpaceType>
class ModelBase
{
public:
    ModelBase() :
        outputMaker(&space),
        time(0)
    {
        space.setParent(this);
        space.initQuantities();
    }
    
    ~ModelBase()
    {
        space.stopThreads();
    }

    SpaceType space;
    
    void iterate(double dt)
    {
        outputMaker.output(time);
        space.calculateEvolution(dt);
        space.addDelta();
        if (counter++ == 10) {
            counter = 0;
            space.averageWithNeighbours();
        }
        time += dt;
    }

    void setThreadsCount(unsigned int count)
    {
        space.initThreads(count);
    }
    
    void run(double stopTime, double timeStep)
    {
        counter=0;
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
        std::cout << std::endl;
    }

protected:
    OutputMaker outputMaker;
    double time;
    int counter;
};

#endif // MODEL_BASE_H_INCLUDED

