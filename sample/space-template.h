/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef SPACE_TEMPLATE_H_INCLUDED
#define SPACE_TEMPLATE_H_INCLUDED

#include "global-defines.h"
#include "fractions-pool-template.h"

class SpaceBase : public SpaceGridType
{
public:
    SpaceBase(void* model) :
        parent(model)
    {}
    
    virtual ~SpaceBase() {}
    
    void calculateFlowsEvolution(double dt);
    void calculateSourceEvolution(double dt);
    void swapBuffers();
    
    void* parent;

protected:
    typename SpaceGridType::GridDescription spaceGridDescription;
};

/*
template<class GridType>
class SpaceBase : public GridType
{
public:
    SpaceBase(void* model) :
        parent(model)
    {}
    
    void swapBuffers()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<typename GridType::CurrentGridElementType*>( &(this->elements[i]))->swapBuffers();
    }
    
    void calculateFlowsEvolution(double dt)
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<typename GridType::CurrentGridElementType*>( &(this->elements[i]))->calculateFlowsEvolution(dt);
    }
    
    void calculateSourceEvolution(double dt)
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<typename GridType::CurrentGridElementType*>( &(this->elements[i]))->calculateSourceEvolution(dt);
    }
    
    void* parent;
};
*/
#endif // SPACE_TEMPLATE_H_INCLUDED
