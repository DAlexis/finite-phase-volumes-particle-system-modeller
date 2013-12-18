#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include "space.h"

class Model
{
public:
    Space space;
    
    void iterate(double dt);
private:
};

#endif // MODEL_H_INCLUDED
