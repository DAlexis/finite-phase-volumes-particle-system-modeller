#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include "space.h"

class Model
{
public:
    Space space;
    
    void iterate(double dt);
    
private:
    /// @todo This descriptions should be used to construct grids
    Fraction1SpaceGridType::GridDescription fraction1GridDescription;
    SpaceGridType::GridDescription spaceGridDescription;
};

#endif // MODEL_H_INCLUDED
