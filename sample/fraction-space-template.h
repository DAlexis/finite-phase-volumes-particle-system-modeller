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
    

protected:
    typename GridType::GridDescription gridDescription;
};

#endif
