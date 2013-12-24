/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTION_SPACE_TEMPLATE_H_INCLUDED
#define FRACTION_SPACE_TEMPLATE_H_INCLUDED

class FractionsPool;

template<class GridInstanceType>
class FractionSpace : public GridInstanceType
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
    typename GridInstanceType::GridDescription frtactionGridDescription;
};

#endif
