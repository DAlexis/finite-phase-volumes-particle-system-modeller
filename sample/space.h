#ifndef LOCATION_SPACE_H_INCLUDED
#define LOCATION_SPACE_H_INCLUDED

#include "global-defines.h"
#include "space-template.h"

class Space : public SpaceBase
{
public:
    Space(void* model);
    virtual ~Space() {}
    
private:
    virtual GridInstance::GridElement* createGridElements(size_t count);
};

/*
class Space : public SpaceBase<SpaceGridType>
{
public:
    Space(void* model);
    
private:
    SpaceGridType::GridDescription spaceGridDescription;
};
*/
#endif // LOCATION_SPACE_H_INCLUDED
