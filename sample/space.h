#ifndef LOCATION_SPACE_H_INCLUDED
#define LOCATION_SPACE_H_INCLUDED

#include "fractions-pool.h"
#include "grid-template.h"

class SpaceGrid : public Grid<1, FractionsPool>
{
public:
    SpaceGrid();
    
private:
    GridDescription<1> gridDescription;
};

#endif // LOCATION_SPACE_H_INCLUDED
