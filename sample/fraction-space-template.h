#ifndef FRACTION_SPACE_TEMPLATE_H_INCLUDED
#define FRACTION_SPACE_TEMPLATE_H_INCLUDED

class FractionsPool;

template<class GridType>
class FractionSpace : public GridType
{
public:
    FractionSpace(FractionsPool* parentFractionsPool) :
        parent(parentFractionsPool)
    {}
    
    FractionsPool* parent;
    
    void swapBuffers()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            this->elements[i].data.swapBuffers();
    }

protected:
    typename GridType::GridDescription gridDescription;
};

#endif
