#include "fraction-1.h"
#include "model.h"


#include <string.h>
#include <algorithm>

Fraction1Cell::Fraction1Cell()
{
    
}

void* Fraction1Cell::getSpaceCell()
{
    /// @todo Remove this temporary pointers and write 1-line optimal code
    FractionsPool* pFractionsPool = static_cast<FractionsPool*>(getFractionPool());
    SpaceGridType::GridElement* pSpaceGridElement = static_cast<SpaceGridType::GridElement*>(pFractionsPool->parent);
    return pSpaceGridElement;
}

void* Fraction1Cell::getFractionPool()
{
    /// @todo Remove this temporary pointers and write 1-line optimal code
    Fraction1SpaceGridType* pFractionGrid = static_cast<Fraction1SpaceGridType*>(parent->parentGrid);
    Fraction1Space* pFractionSpace = static_cast<Fraction1Space*>(pFractionGrid);
    FractionsPool* pFractionsPool = pFractionSpace->parent;
    return pFractionsPool;
}

void* Fraction1Cell::getModel()
{
    SpaceGridType::GridElement* spaceCell = static_cast<SpaceGridType::GridElement*>(getSpaceCell());
    Space* space = static_cast<Space*>(spaceCell->parentGrid);
    return space->parent;
}

void Fraction1Cell::calculateDerivatives()
{
    spaceCoordsDerivatives[SPACE_COORDS_X] = parent->coordinates[FRACTION1_COORDS_VX];
    fractionCoordsDerivatives[FRACTION1_COORDS_VX] = 0;// -9.8;
}

Fraction1Cell* Fraction1Cell::nextInSpace(unsigned int coordinate)
{
    SpaceGridType::GridElement* spaceCell = static_cast<SpaceGridType::GridElement*>(getSpaceCell());
    if (spaceCell->next[coordinate] == NULL)
        return NULL;
    
    return &(spaceCell->next[coordinate]->data.fraction1.elements[parent->elementIndex].data);
}

Fraction1Cell* Fraction1Cell::nextInFractionSpace(unsigned int coordinate)
{
    if (parent->next[coordinate] == NULL)
        return NULL;
    return &(parent->next[coordinate]->data);
}

Fraction1Cell* Fraction1Cell::prevInSpace(unsigned int coordinate)
{
    SpaceGridType::GridElement* spaceCell = static_cast<SpaceGridType::GridElement*>(getSpaceCell());
    if (spaceCell->prev[coordinate] == NULL)
        return NULL;
    
    return &(spaceCell->prev[coordinate]->data.fraction1.elements[parent->elementIndex].data);
}

Fraction1Cell* Fraction1Cell::prevInFractionSpace(unsigned int coordinate)
{
    if (parent->prev[coordinate] == NULL)
        return NULL;
    return &(parent->prev[coordinate]->data);
}

double Fraction1Cell::getFlowInSpace(unsigned int coordinate, unsigned int quantity, TransferDirection direction, Fraction1Cell* neighbor)
{
    SpaceGridType::GridElement* thisSpaceCell = static_cast<SpaceGridType::GridElement*>(getSpaceCell());
    SpaceGridType::GridElement* neighborSpaceCell = static_cast<SpaceGridType::GridElement*>(neighbor->getSpaceCell());
    double l1 = thisSpaceCell->size[coordinate];
    double l2 = neighborSpaceCell->size[coordinate];
    
    double borderValue = (quantities[quantity]*l2 + neighbor->quantities[quantity]*l1)
        / (l1 + l2);
    double borderCoordDerivative = (spaceCoordsDerivatives[coordinate]*l2 + neighbor->spaceCoordsDerivatives[coordinate]*l1)
        / (l1 + l2);
    double flow = borderValue*borderCoordDerivative / l1;
    
    if (direction == TD_DOWN)
        flow = -flow;
    
    return flow;
}

double Fraction1Cell::getFlowInFractionSpace(unsigned int coordinate, unsigned int quantity, TransferDirection direction, Fraction1Cell* neighbor)
{
    double l1 = parent->size[coordinate];
    double l2 = neighbor->parent->size[coordinate];
    
    double borderValue = (quantities[quantity]*l2 + neighbor->quantities[quantity]*l1)
        / (l1 + l2);
    double borderCoordDerivative = (fractionCoordsDerivatives[coordinate]*l2 + neighbor->fractionCoordsDerivatives[coordinate]*l1)
        / (l1 + l2);
    double flow = borderValue*borderCoordDerivative / l1;
    
    if (direction == TD_DOWN)
        flow = -flow;
    
    return flow;
}

void Fraction1Cell::calculateFlowsEvolution(double dt)
{
    /// @todo Implement here for multi-sign values (like charge)
    /// @todo optimize here?
    
    // For each quantity
    double flowOutUp[FRACTION1_COORDS_COUNT + SPACE_COORDS_COUNT];
    double flowOutDown[FRACTION1_COORDS_COUNT + SPACE_COORDS_COUNT];
    double totalFlowOut = 0;
    memset( flowOutUp, 0, sizeof(double) * (FRACTION1_COORDS_COUNT+SPACE_COORDS_COUNT) );
    memset( flowOutDown, 0, sizeof(double) * (FRACTION1_COORDS_COUNT+SPACE_COORDS_COUNT) );
    
    // Flows in fraction space
    for (uint coord=0; coord<FRACTION1_COORDS_COUNT; coord++)
    {
        Fraction1Cell* next = nextInFractionSpace(coord);
        Fraction1Cell* prev = prevInFractionSpace(coord);
        if (next)
        {
            /// @todo Interpolation should be added here
            double transfer = getFlowInFractionSpace(coord, EVERY_FRACTION_COUNT_QUANTITY_INDEX, TD_UP, next) *dt;
            if (transfer > 0)
            {
                // Flow out from this cell in positive coordinate direction
                flowOutUp[coord] = transfer;
                totalFlowOut += transfer;
            }
        }
        
        if (prev)
        {
            double transfer = getFlowInFractionSpace(coord, EVERY_FRACTION_COUNT_QUANTITY_INDEX, TD_DOWN, prev) *dt;
            if (transfer > 0)
            {
                // Flow out from this cell in negative coordinate direction
                flowOutDown[coord] = transfer;
                totalFlowOut += transfer;
            }
        }
    }
    
    // Flows in coordinate space
    for (uint coord=0; coord<SPACE_COORDS_COUNT; coord++)
    {
        Fraction1Cell* next = nextInSpace(coord);
        Fraction1Cell* prev = prevInSpace(coord);
        if (next)
        {
            double transfer = getFlowInSpace(coord, EVERY_FRACTION_COUNT_QUANTITY_INDEX, TD_UP, next) * dt;
            if (transfer > 0)
            {
                // Flow out from this cell in positive coordinate direction
                flowOutUp[FRACTION1_COORDS_COUNT + coord] = transfer;
                totalFlowOut += transfer;
            }
        }
        if (prev)
        {
            double transfer = getFlowInSpace(coord, EVERY_FRACTION_COUNT_QUANTITY_INDEX, TD_DOWN, prev) * dt;
            if (transfer > 0)
            {
                // Flow out from this cell in negative coordinate direction
                flowOutDown[FRACTION1_COORDS_COUNT + coord] = transfer;
                totalFlowOut += transfer;
            }
        }
    }
    
    double rewnormCoefficient = 1;
    
    // q[i]/n
    double quantityOverParticlesCount[FRACTION1_QUANTITIES_COUNT];
    for (unsigned int quantity=0; quantity<FRACTION1_QUANTITIES_COUNT; quantity++)
        if (isNotNull(quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]))
            quantityOverParticlesCount[quantity] = quantities[quantity] / quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX];
        else
            quantityOverParticlesCount[quantity] = 0;
    
    // Removing flowed out quantities including particles count
    if (totalFlowOut > quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX])
    {
        rewnormCoefficient = quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX] / totalFlowOut;
        for (unsigned int quantity=0; quantity<FRACTION1_QUANTITIES_COUNT; quantity++)
            nextStepQuantities[quantity] = 0;
    } else {
        for (unsigned int quantity=0; quantity<FRACTION1_QUANTITIES_COUNT; quantity++)
            nextStepQuantities[quantity] -= totalFlowOut * quantityOverParticlesCount[quantity];

    }
    
    // Adding quantities to neighbors
    for (unsigned int quantity=0; quantity<FRACTION1_QUANTITIES_COUNT; quantity++)
    {
        // In fraction space
        for (uint coord=0; coord<FRACTION1_COORDS_COUNT; coord++)
        {
            Fraction1Cell *next = nextInFractionSpace(coord);
            if (next) next->nextStepQuantities[quantity] += flowOutUp[coord] * rewnormCoefficient * quantityOverParticlesCount[quantity];
            
            Fraction1Cell *prev = prevInFractionSpace(coord);
            if (prev) prev->nextStepQuantities[quantity] += flowOutDown[coord] * rewnormCoefficient * quantityOverParticlesCount[quantity];
        }
        // In coordinate space
        for (uint coord=0; coord<SPACE_COORDS_COUNT; coord++)
        {
            Fraction1Cell *next = nextInSpace(coord);
            if (next) next->nextStepQuantities[quantity] += flowOutUp[FRACTION1_COORDS_COUNT+coord] * rewnormCoefficient * quantityOverParticlesCount[quantity];
            
            Fraction1Cell *prev = prevInSpace(coord);
            if (prev) prev->nextStepQuantities[quantity] += flowOutDown[FRACTION1_COORDS_COUNT+coord] * rewnormCoefficient * quantityOverParticlesCount[quantity];
        }
    }
}

void Fraction1Cell::calculateSourceEvolution(double dt)
{
    SpaceGridType::GridElement* spaceCell = static_cast<SpaceGridType::GridElement*>(getSpaceCell());
    
    if (static_cast<Model*>(getModel())->time < 0.02)
    
    if (fabs(parent->coordinates[FRACTION1_COORDS_VX]+3) < 0.5)
    //if (fabs(parent->coordinates[FRACTION1_COORDS_VX]) < 10)
    if (fabs(spaceCell->coordinates[SPACE_COORDS_X]-8) < 1)
    //    nextStepQuantities[FRACTION1_QUANTITY_COUNT] += 0.1*spaceCell->volume*dt;
    nextStepQuantities[FRACTION1_QUANTITY_COUNT] = 1;//spaceCell->coordinates[SPACE_COORDS_X]/30;
    
    //else
    //nextStepQuantities[FRACTION1_QUANTITY_COUNT] = 0;
}

Fraction1Space::Fraction1Space(FractionsPool* parentFractionsPool) :
    FractionSpace(parentFractionsPool)
{
    // Setting up x coordinate
    Axis& xSpeed = frtactionGridDescription.axis[0];
    xSpeed.uniformInit(-10.0, 10.0, 100);
    xSpeed.setName("Horisontal speed, m");
    
    constructGrid(frtactionGridDescription);
}

void Fraction1Space::calculateFlowsEvolution(double dt)
{
    for (size_t i=0; i<elementsCount; i++)
        elements[i].data.calculateDerivatives();
    
    for (size_t i=0; i<elementsCount; i++)
        elements[i].data.calculateFlowsEvolution(dt);
}

void Fraction1Space::calculateSourceEvolution(double dt)
{
     for (size_t i=0; i<elementsCount; i++)
        elements[i].data.calculateSourceEvolution(dt);
}
