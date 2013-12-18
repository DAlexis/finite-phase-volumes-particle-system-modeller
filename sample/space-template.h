#ifndef SPACE_TEMPLATE_H_INCLUDED
#define SPACE_TEMPLATE_H_INCLUDED

template<class GridType>
class SpaceBase : public GridType
{
public:
    void swapBuffers()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            this->elements[i].data.swapBuffers();
    }
    
    void calculateFlowsEvolution(double dt)
    {
        for (size_t i=0; i<this->elementsCount; i++)
            this->elements[i].data.calculateFlowsEvolution(dt);
    }
    
    void calculateSourceEvolution(double dt)
    {
        for (size_t i=0; i<this->elementsCount; i++)
            this->elements[i].data.calculateSourceEvolution(dt);
    }
    
};

#endif // SPACE_TEMPLATE_H_INCLUDED
