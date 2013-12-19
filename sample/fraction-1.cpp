#include "fraction-1.h"
#include "space.h"

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
            double flow = borderValue*borderCoordDerivative*dt;
            
            // We suppose that quantity could be only positive
            if (flow > 0)
                flow = std::min(flow, quantities[quantity]);
            else
                flow = std::min(flow, next.quantities[quantity]);
            
            nextStepQuantities[quantity] -= flow;
            next.nextStepQuantities[quantity] += flow;
        }
        
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
                flow = std::min(flow, next.quantities[quantity]);
            
            nextStepQuantities[quantity] -= flow;
            next.nextStepQuantities[quantity] += flow;
        }
    }
}

void Fraction1Cell::calculateSourceEvolution(double dt)
{
    SpaceGridType::GridElement* spaceCell = static_cast<SpaceGridType::GridElement*>(getSpaceCell());
    //if (spaceCell->coordinates[SPACE_COORDS_X] <0)
        nextStepQuantities[FRACTION1_QUANTITY_CONCENTRATION] += 0.1*spaceCell->volume*dt;
}

Fraction1Space::Fraction1Space(FractionsPool* parentFractionsPool) :
    FractionSpace(parentFractionsPool)
{
    // Setting up x coordinate
    Axis& xSpeed = gridDescription.axis[0];
    xSpeed.uniformInit(-200.0, 100.0, 10);
    xSpeed.setName("Horisontal speed, m");
    
    constructGrid(gridDescription);
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
