#include "fraction-1.h"
#include "model.h"

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
    fractionCoordsDerivatives[FRACTION1_COORDS_VX] = -9.8;
}

void Fraction1Cell::calculateFlowsEvolution(double dt)
{
    /// @todo Implement here for multi-sign values (like charge)
    /// @todo optimize here?
    
    // For each quantity
    for (uint quantity=0; quantity<FRACTION1_QUANTITIES_COUNT; quantity++)
    {
        // Flows in fraction space
        for (uint coord=0; coord<FRACTION1_COORDS_COUNT; coord++)
        {
            if (parent->next[coord] == NULL) continue;
            
            Fraction1Cell& next = parent->next[coord]->data;
            
            /// @todo Interpolation should be added here
            double borderValue = (quantities[quantity]
                + next.quantities[quantity])
                / 2;
            
            double borderCoordDerivative = (fractionCoordsDerivatives[coord]
                + next.fractionCoordsDerivatives[coord])
                / 2;
            
            // Flow out from this cell in positive coordinate direction
            double totalTransfer = borderValue*borderCoordDerivative*dt;
            
            // We suppose that quantity could be only positive
            if (totalTransfer > 0)
                totalTransfer = std::min(totalTransfer, quantities[quantity]);
            else
                totalTransfer = std::max(totalTransfer, -next.quantities[quantity]);
            
            nextStepQuantities[quantity] -= totalTransfer;
            next.nextStepQuantities[quantity] += totalTransfer;
        }
        /*
        // Flows in coordinate space
        for (uint coord=0; coord<SPACE_COORDS_COUNT; coord++)
        {
            SpaceGridType::GridElement* spaceCell = static_cast<SpaceGridType::GridElement*>(getSpaceCell());
            if (spaceCell->next[coord] == NULL) continue;
            
            Fraction1Cell& next = spaceCell->next[coord]->data.fraction1.elements[parent->elementIndex].data;
            
            double borderValue = (quantities[quantity]
                + next.quantities[quantity])
                / 2;
            
            double borderCoordDerivative = (spaceCoordsDerivatives[coord]
                + next.spaceCoordsDerivatives[coord])
                / 2;
            
            // Flow out from this cell in positive coordinate direction
            double flow = borderValue*borderCoordDerivative*dt;
            
            // We suppose that quantity could be only positive
            if (flow > 0)
                flow = std::min(flow, quantities[quantity]);
            else
                flow = std::max(flow, -next.quantities[quantity]);
            
            nextStepQuantities[quantity] -= flow;
            next.nextStepQuantities[quantity] += flow;
        }*/
    }
}

void Fraction1Cell::calculateSourceEvolution(double dt)
{
    SpaceGridType::GridElement* spaceCell = static_cast<SpaceGridType::GridElement*>(getSpaceCell());
    
    if (static_cast<Model*>(getModel())->time < 0.02 && fabs(parent->coordinates[FRACTION1_COORDS_VX]) < 1)
    
    //if (fabs(parent->coordinates[FRACTION1_COORDS_VX]) < 10)
    //if (spaceCell->coordinates[SPACE_COORDS_X] <0)
    //    nextStepQuantities[FRACTION1_QUANTITY_CONCENTRATION] += 0.1*spaceCell->volume*dt;
    nextStepQuantities[FRACTION1_QUANTITY_CONCENTRATION] = 1;//spaceCell->coordinates[SPACE_COORDS_X]/30;
    
    //else
    //nextStepQuantities[FRACTION1_QUANTITY_CONCENTRATION] = 0;
}

Fraction1Space::Fraction1Space(FractionsPool* parentFractionsPool) :
    FractionSpace(parentFractionsPool)
{
    // Setting up x coordinate
    Axis& xSpeed = frtactionGridDescription.axis[0];
    xSpeed.uniformInit(-20.0, 10.0, 30);
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
