#ifndef LOCATION_SPACE_H_INCLUDED
#define LOCATION_SPACE_H_INCLUDED

#include "global-defines.h"
#include "fractions-pool.h"

class Space : public SpaceGridType
{
public:
    Space(void* model);
    
    virtual ~Space() {}
    
    void calculateFlowsEvolution(double dt);
    void calculateSourceEvolution(double dt);
    void swapBuffers();
    
private:
    void* parent;
    typename SpaceGridType::GridDescription spaceGridDescription;
};

#endif // LOCATION_SPACE_H_INCLUDED
