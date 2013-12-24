/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTION_CELL_TEMPLATE
#define FRACTION_CELL_TEMPLATE

#include "global-defines.h"
#include "fractions-pool-template.h"
#include "fraction-space-template.h"

template <int FractionIndex,
          int SpaceDimension,
          int FractionSpaceDimension,
          int QuantitiesCount,
          class GridInstanceType>
class FractionCell
{
public: 
    FractionCell()
    {
        for (unsigned int i=0; i<QuantitiesCount; i++) {
            quantitiesBuffer0[i] = 0;
            quantitiesBuffer1[i] = 0;
        }
        quantities = quantitiesBuffer0;
        nextStepQuantities = quantitiesBuffer1;
    }
    
    void swapBuffers()
    {
        if (quantities == quantitiesBuffer0)
        {
            quantities = quantitiesBuffer1;
            nextStepQuantities = quantitiesBuffer0;
        } else {
            quantities = quantitiesBuffer0;
            nextStepQuantities = quantitiesBuffer1;
        }
        for (unsigned int i=0; i<QuantitiesCount; i++)
        {
            nextStepQuantities[i] = quantities[i];
        }
    }
    
    FractionsPoolTemplate* getFractionPool()
    {
        /// @todo Remove this temporary pointers and write 1-line optimal code
        GridInstanceType* pFractionGrid = static_cast<GridInstanceType*>(parent->parentGrid);
        FractionSpace<GridInstanceType>* pFractionSpace = static_cast<FractionSpace<GridInstanceType>*>(pFractionGrid);
        FractionsPoolTemplate* pFractionsPool = pFractionSpace->parent;
        return pFractionsPool;
    }

    
    SpaceGridType::GridElement* getSpaceCell()
    {
        /// @todo Remove this temporary pointers and write 1-line optimal code
        FractionsPoolTemplate* pFractionsPool = static_cast<FractionsPoolTemplate*>(getFractionPool());
        SpaceGridType::GridElement* pSpaceGridElement = static_cast<SpaceGridType::GridElement*>(pFractionsPool->parent);
        return pSpaceGridElement;
    }
    
    Fraction1Cell* nextInSpaceTMP(unsigned int coordinate)
    {
        /*
        SpaceGridType::GridElement* spaceCell = getSpaceCell();
        if (spaceCell->next[coordinate] == NULL)
            return NULL;
        
        return &(spaceCell->next[coordinate]->data.fraction1.elements[parent->elementIndex].data);*/
        return NULL;
    }
    
    void init(typename GridInstanceType::GridElement* parentGridElement) { parent = parentGridElement; }
    
    double* quantities;
    double* nextStepQuantities;
    
     /// Space coordinates derivatives
    double spaceCoordsDerivatives[SpaceDimension];
    /// Fraction coordinates derivatives
    double fractionCoordsDerivatives[FractionSpaceDimension];
    
protected:
    typename GridInstanceType::GridElement *parent;
    
    enum TransferDirection
    {
        TD_UP = 0,
        TD_DOWN
    };
    
private:
    double quantitiesBuffer0[QuantitiesCount];
    double quantitiesBuffer1[QuantitiesCount];
};

#endif // FRACTION_CELL_TEMPLATE
