/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include "space.h"
#include "model-output.h"

class Model
{
public:
    Model();
    Space space;
    
    void iterate(double dt);
    OutputMaker outputMaker;
    
    double time;
};

#endif // MODEL_H_INCLUDED
