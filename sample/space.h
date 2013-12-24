#ifndef LOCATION_SPACE_H_INCLUDED
#define LOCATION_SPACE_H_INCLUDED

#include "fractions-pool.h"
#include "grid-template.h"
#include "global-defines.h"
#include "space-template.h"

class Space : public SpaceBase<SpaceGridType>
{
public:
    Space(void* model);
    
private:
    SpaceGridType::GridDescription spaceGridDescription;
};

#endif // LOCATION_SPACE_H_INCLUDED
