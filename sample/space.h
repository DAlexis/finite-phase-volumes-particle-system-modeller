#ifndef LOCATION_SPACE_H_INCLUDED
#define LOCATION_SPACE_H_INCLUDED

#include "fractions-pool.h"
#include "grid-template.h"
#include "global-defines.h"

typedef Grid<SPACE_DIMENSION, FractionsPool> SpaceGridType;

class Space : public SpaceGridType
{
public:
    enum SpaceCoordinates {
        SPACE_COORDS_X = 0,
        SPACE_COORDS_COUNT /// @todo This duplicate global define. May be I need to move it to global-defines.h
    };
    
    Space();
    
private:
    SpaceGridType::GridDescription gridDescription;
};

#endif // LOCATION_SPACE_H_INCLUDED
