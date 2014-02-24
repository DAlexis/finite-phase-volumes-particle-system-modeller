/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef MODEL_BASE_H_INCLUDED
#define MODEL_BASE_H_INCLUDED

#include "space.h"
#include "output.h"

class ModelBase
{
public:
    ModelBase();
    Space space;
    
    void iterate(double dt);
    OutputMaker outputMaker;
    
    double time;
    void setThreadsCount(unsigned int count);
};

#endif // MODEL_BASE_H_INCLUDED

