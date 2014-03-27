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
        time(0),
        lastStabilisationTime(0)
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
        if ( time-lastStabilisationTime >= stabilisationPeriod )
        {
            lastStabilisationTime = time;
            space.averageWithNeighbours();
        }
        time += dt;
    }

    void setThreadsCount(unsigned int count)
    {
        space.initThreads(count);
    }
    
    void setStabilisationPeriod(const double period)
    {
        stabilisationPeriod = period;
    }
    
    void run(double stopTime, double timeStep)
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
        std::cout << std::endl;
    }

protected:
    OutputMaker outputMaker;
    double time;
    double lastStabilisationTime;
    double stabilisationPeriod;
};

#endif // MODEL_BASE_H_INCLUDED

