#ifndef SPACE_H_INCLUDED
#define SPACE_H_INCLUDED

#include "global-defines.h"
#include "fractions-pool.h"
#include "space-base.h"

class Space : public SpaceBase<SPACE_COORDS_COUNT, FractionsPool>
{
public:
    Space();

};

#endif // SPACE_H_INCLUDED
