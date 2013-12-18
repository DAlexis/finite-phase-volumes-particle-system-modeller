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

Fraction1Space::Fraction1Space(FractionsPool* parentFractionsPool) :
    FractionSpace(parentFractionsPool)
{
    // Setting up x coordinate
    Axis& xSpeed = gridDescription.axis[0];
    xSpeed.uniformInit(-100.0, 100.0, 10);
    xSpeed.setName("Horisontal speed, m");
    
    constructGrid(gridDescription);
}

void Fraction1Space::calculateFlowsEvolution(double dt)
{
    // Simpliest way. May be cirles need to be merged
    for (size_t i=0; i<elementsCount; i++)
    {
        elements[i].data.calculateDerivatives();
    }
    for (size_t i=0; i<elementsCount; i++)
    {
        /// @todo implement here
        /// @todo optimize here
        
        for (uint quantity=0; quantity<FRACTION1_QUANTITIES_COUNT; quantity++)
        {
            elements[i].data.nextStepQuantities[quantity] = elements[i].data.quantities[quantity];
            // Counting flow for each coordinate
            for (uint coord=0; coord<FRACTION1_COORDS_COUNT; coord++)
            {
                /// Flow from this to next
                if (elements[i].next[i] == NULL) continue;
                
                /// @todo Interpolation should be added here
                double borderValue = (elements[i].data.quantities[quantity]
                    + elements[i].next[coord]->data.quantities[quantity])
                    / 2;
                
                double borderDerivative = (elements[i].data.fractionCoordsDerivatives[coord]
                    + elements[i].next[coord]->data.fractionCoordsDerivatives[coord])
                    / 2;
                
                double flow = borderValue*borderDerivative*dt;
                
                // We suppose that quantity could be only positive
                if (flow > 0)
                    flow = std::min(flow, elements[i].data.quantities[quantity]);
                else
                    flow = std::min(flow, elements[i].next[coord]->data.quantities[quantity]);
                /// @todo I need to enter connected values
                elements[i].data.nextStepQuantities[quantity] -= flow;
                elements[i].next[coord]->data.nextStepQuantities[quantity] += flow;
            }
        }
    }
}
