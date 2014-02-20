/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef SPACE_BASE_H_INCLUDED
#define SPACE_BASE_H_INCLUDED

#include "global-defines.h"

template<int SpaceDimension, class GridElementType>
class SpaceBase : public Grid<SpaceDimension, GridElementType>
{
public:
    typedef Grid<SpaceDimension, GridElementType> SpaceGridInstance;
    SpaceBase() { }
    
    virtual ~SpaceBase() {}
    
    void initQuantities()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            this->elements[i].initQuantities();
    }

    void calculateEvolution(double dt)
    {
        for (size_t i=0; i<this->elementsCount; i++)
        {
             this->elements[i].calculateFlowsEvolution(dt);
             this->elements[i].calculateSourceEvolution(dt);
        }
    }

    void swapBuffers()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            this->elements[i].swapBuffers();
    }

    void setParent(void* model) {parent = model;}
    void* parent;
    
    typename SpaceGridInstance::GridDescription spaceGridDescription;
    
};


#endif // SPACE_BASE_H_INCLUDED
