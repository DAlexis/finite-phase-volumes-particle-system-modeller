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
//    OutputMaker outputMaker;
    //void outputTimeSlice(const Fraction fraction, const int quantity, const int axis);
    
    double time;
private:
    /// @todo This descriptions should be used to construct grids
    //Fraction1SpaceGridType::GridDescription fraction1GridDescription;
    //SpaceGridType::GridDescription spaceGridDescription;
};

#endif // MODEL_H_INCLUDED
