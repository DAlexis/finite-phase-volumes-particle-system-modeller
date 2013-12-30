/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTION_CELL_TEMPLATE
#define FRACTION_CELL_TEMPLATE

#include "global-defines.h"
#include "fractions-pool-base.h"
#include "fraction-cell-interface.h"
#include "fraction-space-interface.h"

template <int AxisCount, class FractionCellType>
class FractionSpaceBase : public Grid<AxisCount, FractionCellType>, public IFractionSpace
{
public:
    
    FractionSpaceBase(FractionsPoolBase* parentFractionsPool) :
        parent(parentFractionsPool)
    {}
    
    virtual ~FractionSpaceBase() { }
    
    virtual void calculateFlowsEvolution(double dt)
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateDerivatives();
        
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateFlowsEvolution(dt);
    }
    
    virtual void calculateSourceEvolution(double dt)
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateSourceEvolution(dt);
    }
    
    virtual void swapBuffers()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->swapBuffers();
    }
    
    FractionsPoolBase* parent;

protected:
    typename Grid<AxisCount, FractionCellType>::GridDescription fractionGridDescription;
};

template <int FractionIndex,
          int SpaceDimension,
          int FractionSpaceDimension,
          int QuantitiesCount,
          class FractionCellType>
class FractionCellBase : public Grid<SpaceDimension, FractionCellType>::GridElementBase, public IFractionCell
{
public:
    typedef Grid<SpaceDimension, FractionCellType> GridInstance;
    
    FractionCellBase()
    {
        for (unsigned int i=0; i<QuantitiesCount; i++) {
            quantitiesBuffer0[i] = 0;
            quantitiesBuffer1[i] = 0;
        }
        quantities = quantitiesBuffer0;
        nextStepQuantities = quantitiesBuffer1;
    }
    
    virtual ~FractionCellBase() { }
    
    virtual void swapBuffers()
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
    
    virtual void calculateFlowsEvolution(double dt)
    {
    }
    
    double* quantities;
    double* nextStepQuantities;
    
     /// Space coordinates derivatives
    double spaceCoordsDerivatives[SpaceDimension];
    /// Fraction coordinates derivatives
    double fractionCoordsDerivatives[FractionSpaceDimension];
    
protected:
    enum TransferDirection
    {
        TD_UP = 0,
        TD_DOWN
    };
    
private:
    double quantitiesBuffer0[QuantitiesCount];
    double quantitiesBuffer1[QuantitiesCount];
};

/*
template <int FractionIndex,
          int SpaceDimension,
          int FractionSpaceDimension,
          int QuantitiesCount,
          class GridInstanceType>
class FractionCell : public GridInstanceType::GridElement
{
public:
    
    FractionsPoolTemplate* getFractionPool()
    {
        /// @todo Remove this temporary pointers and write 1-line optimal code
        // Why needed static cast???
        FractionSpace<GridInstanceType>* pFractionSpace = static_cast< FractionSpace<GridInstanceType>* > (this->parentGrid);
        FractionsPoolTemplate* pFractionsPool = pFractionSpace->parent;
        return pFractionsPool;
    }

    ///////////////////////////
    // Next/Prev cells getting
    FractionCell* nextInSpace(unsigned int coordinate)
    {
        FractionsPoolTemplate* fractionsPool = getFractionPool();
        if (fractionsPool->next[coordinate] == NULL)
            return NULL;
        
        void *fraction = static_cast<FractionsPoolTemplate*>(fractionsPool->next[coordinate])->fractions[FractionIndex];
        return static_cast<FractionCell*> ( &( static_cast<GridInstanceType*>(fraction)->elements[this->elementIndex] ));
    }
    
    FractionCell* prevInSpace(unsigned int coordinate)
    {
        FractionsPoolTemplate* fractionsPool = getFractionPool();
        if (fractionsPool->prev[coordinate] == NULL)
            return NULL;
        
        void *fraction = static_cast<FractionsPoolTemplate*>(fractionsPool->prev[coordinate])->fractions[FractionIndex];
        return static_cast<FractionCell*> ( &( static_cast<GridInstanceType*>(fraction)->elements[this->elementIndex] ));
    }
    
    FractionCell* nextInFractionSpace(unsigned int coordinate)
    {
        return  static_cast<FractionCell*>(static_cast<typename GridInstanceType::GridElement*>(this)->next[coordinate]);
    }
    
    FractionCell* prevInFractionSpace(unsigned int coordinate)
    {
        return  static_cast<FractionCell*>(static_cast<typename GridInstanceType::GridElement*>(this)->prev[coordinate]);
    }
    
    
};
*/
#endif // FRACTION_CELL_TEMPLATE
