#ifndef LOCATION_SPACE_H_INCLUDED
#define LOCATION_SPACE_H_INCLUDED

#include "fractions-pool.h"
#include "grid-template.h"
#include "global-defines.h"

typedef Grid<SPACE_COORDS_COUNT, FractionsPool> SpaceGridType;

class Space : public SpaceGridType
{
public:
    Space();
    
private:
    SpaceGridType::GridDescription gridDescription;
};

#endif // LOCATION_SPACE_H_INCLUDED
