/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef SPACE_TEMPLATE_H_INCLUDED
#define SPACE_TEMPLATE_H_INCLUDED

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

#endif // SPACE_TEMPLATE_H_INCLUDED
