#ifndef LOCATION_SPACE_H_INCLUDED
#define LOCATION_SPACE_H_INCLUDED

#include "fractions-pool.h"
#include "grid-template.h"
#include "global-defines.h"
#include "space-template.h"

typedef Grid<SPACE_COORDS_COUNT, FractionsPool> SpaceGridType;

class Space : public SpaceBase<SpaceGridType>
{
public:
    Space();
    
private:
    SpaceGridType::GridDescription spaceGridDescription;
};

#endif // LOCATION_SPACE_H_INCLUDED
