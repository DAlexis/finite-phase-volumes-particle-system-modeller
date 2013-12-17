#ifndef LOCATION_SPACE_H_INCLUDED
#define LOCATION_SPACE_H_INCLUDED

#include "fractions-pool.h"
#include "grid-template.h"

typedef Grid<1, FractionsPool> SpaceGridType;

class Space : SpaceGridType
{
public:
    Space();
    
private:
    SpaceGridType::GridDescription gridDescription;
};

#endif // LOCATION_SPACE_H_INCLUDED
