/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTION_CELL_TEMPLATE
#define FRACTION_CELL_TEMPLATE

#include "global-defines.h"
#include "fractions-pool-base.h"
#include "fraction-cell-interface.h"
#include "fraction-space-interface.h"

#include <string.h>

/// This is a part of FractionSpaceBase that is indifferent to case AxisCount == 0
template <int AxisCount, class FractionCellType>
class FractionSpaceBaseIncomplete : public Grid<AxisCount, FractionCellType>, public IFractionSpace
{
public:
    
    FractionSpaceBaseIncomplete(FractionsPoolBase* parentFractionsPool) :
        parent(parentFractionsPool)
    {}
    
    virtual ~FractionSpaceBaseIncomplete() { }
    
    void calculateFlowsEvolution(double dt)
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateDerivatives();
        
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateFlowsEvolution(dt);
    }
    
    void calculateSourceEvolution(double dt)
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateSourceEvolution(dt);
    }
    
    void swapBuffers()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->swapBuffers();
    }
    
    IFractionCell* getCell(const double* coords)
    {
        return static_cast<IFractionCell*> (this->accessElement_d(coords));
    }
    
    double getQuantitiesSum(unsigned int quantityIndex)
    {
        double sum=0;
        for (unsigned int i=0; i<this->elementsCount; i++)
            sum += this->elements[i].quantities[quantityIndex];
        return sum;
    }
    
    //const Axis* getAxisDescription(unsigned int axis);
    
    FractionsPoolBase* parent;

protected:
    typename Grid<AxisCount, FractionCellType>::GridDescription fractionGridDescription;
};


template <int AxisCount, class FractionCellType>
class FractionSpaceBase : public FractionSpaceBaseIncomplete<AxisCount, FractionCellType>
{
public:
    FractionSpaceBase(FractionsPoolBase* parentFractionsPool) :
        FractionSpaceBaseIncomplete<AxisCount, FractionCellType>(parentFractionsPool)
    {}
    
    virtual ~FractionSpaceBase() {}
    typedef FractionSpaceBase<AxisCount, FractionCellType> FractionSpaceBaseInstance;
    typedef Grid<AxisCount, FractionCellType> GridInstance;
    
    const Axis* getAxisDescription(unsigned int axis) { return &(this->gridDescription->axis[axis]); }
};

template <class FractionCellType>
class FractionSpaceBase<0, FractionCellType> : public FractionSpaceBaseIncomplete<0, FractionCellType>
{
public:
    FractionSpaceBase(FractionsPoolBase* parentFractionsPool) :
        FractionSpaceBaseIncomplete<0, FractionCellType>(parentFractionsPool)
    {}
    
    virtual ~FractionSpaceBase() {}
    typedef FractionSpaceBase<0, FractionCellType> FractionSpaceBaseInstance;
    typedef Grid<0, FractionCellType> GridInstance;
    
    /// This function is not ever called and is here only to prevent compilation errors
    const Axis* getAxisDescription(unsigned int axis) { return NULL; }
};

template <int FractionIndex,
          int SpaceDimension,
          int FractionSpaceDimension,
          int QuantitiesCount,
          class FractionCellType>
class FractionCellBase : public Grid<FractionSpaceDimension, FractionCellType>::GridElementBase, public IFractionCell
{
public:
    typedef FractionCellBase FractionCellBaseInstance;
    typedef FractionSpaceBase<FractionSpaceDimension, FractionCellType> FractionSpaceBaseInstance;
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
    
    void calculateFlowsEvolution(double dt)
    {
        /// @todo Implement here for multi-sign values (like charge)
        /// @todo optimize here?
        // For each quantity
        double flowOutUp[FractionSpaceDimension + SpaceDimension];
        double flowOutDown[FractionSpaceDimension + SpaceDimension];
        double totalFlowOut = 0;
        memset( flowOutUp, 0, sizeof(double) * (FractionSpaceDimension+SpaceDimension) );
        memset( flowOutDown, 0, sizeof(double) * (FractionSpaceDimension+SpaceDimension) );
        
        // Flows in fraction space
        for (uint coord=0; coord<FractionSpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInFractionSpace(coord);
            FractionCellBaseInstance* prev = prevInFractionSpace(coord);
            if (next)
            {
                /// @todo Interpolation should be added here
                double transfer = getFlowInFractionSpace(coord, next) *dt;
                if (transfer > 0)
                {
                    // Flow out from this cell in positive coordinate direction
                    flowOutUp[coord] = transfer;
                    totalFlowOut += transfer;
                }
            }
            
            if (prev)
            {
                double transfer = -getFlowInFractionSpace(coord, prev) *dt;
                if (transfer > 0)
                {
                    // Flow out from this cell in negative coordinate direction
                    flowOutDown[coord] = transfer;
                    totalFlowOut += transfer;
                }
            }
        }
        
        // Flows in coordinate space
        for (uint coord=0; coord<SpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInSpace(coord);
            FractionCellBaseInstance* prev = prevInSpace(coord);
            if (next)
            {
                double transfer = getFlowInSpace(coord, next) * dt;
                if (transfer > 0)
                {
                    // Flow out from this cell in positive coordinate direction
                    flowOutUp[FractionSpaceDimension + coord] = transfer;
                    totalFlowOut += transfer;
                }
            }
            if (prev)
            {
                double transfer = -getFlowInSpace(coord, prev) * dt;
                if (transfer > 0)
                {
                    // Flow out from this cell in negative coordinate direction
                    flowOutDown[FractionSpaceDimension + coord] = transfer;
                    totalFlowOut += transfer;
                }
            }
        }
        
        double rewnormCoefficient = 1;
        
        // q[i]/n
        double quantityOverParticlesCount[QuantitiesCount];
        /// @todo May be coefficients could be optimized to be more understandable at least
        /// @todo move if out from circle
        for (unsigned int quantity=0; quantity<QuantitiesCount; quantity++)
            if (isNotNull(quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]))
                quantityOverParticlesCount[quantity] = quantities[quantity] / quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX];
            else
                quantityOverParticlesCount[quantity] = 0;
        
        // Removing flowed out quantities including particles count
        if (totalFlowOut > quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX])
        {
            // All particles moved out
            rewnormCoefficient = quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX] / totalFlowOut;
            for (unsigned int quantity=0; quantity<QuantitiesCount; quantity++)
                nextStepQuantities[quantity] = 0;
        } else {
            for (unsigned int quantity=0; quantity<QuantitiesCount; quantity++)
                nextStepQuantities[quantity] -= totalFlowOut * quantityOverParticlesCount[quantity];
        }
        
        // Adding quantities to neighbors
        for (unsigned int quantity=0; quantity<QuantitiesCount; quantity++)
        {
            // In fraction space
            for (uint coord=0; coord<FractionSpaceDimension; coord++)
            {
                FractionCellBaseInstance *next = nextInFractionSpace(coord);
                if (next) next->nextStepQuantities[quantity] += flowOutUp[coord] * rewnormCoefficient * quantityOverParticlesCount[quantity];
                
                FractionCellBaseInstance *prev = prevInFractionSpace(coord);
                if (prev) prev->nextStepQuantities[quantity] += flowOutDown[coord] * rewnormCoefficient * quantityOverParticlesCount[quantity];
            }
            // In coordinate space
            for (uint coord=0; coord<SpaceDimension; coord++)
            {
                FractionCellBaseInstance *next = nextInSpace(coord);
                if (next) next->nextStepQuantities[quantity] += flowOutUp[FractionSpaceDimension+coord] * rewnormCoefficient * quantityOverParticlesCount[quantity];
                
                FractionCellBaseInstance *prev = prevInSpace(coord);
                if (prev) prev->nextStepQuantities[quantity] += flowOutDown[FractionSpaceDimension+coord] * rewnormCoefficient * quantityOverParticlesCount[quantity];
            }
        }
    }
    
    double getQuantitiesDensity(unsigned int index)
    {
        return quantities[index] / this->volume;
    }
    
    double getQuantitiesDensityConvolution(unsigned int quantityIndex, const std::vector<unsigned int>& convoluteBy)
    {
        double result = quantities[quantityIndex];
        double coefficient = 1 / this->volume;
        for (auto it=convoluteBy.begin(); it != convoluteBy.end(); it++)
        {
            // Convolution by one dimension
            coefficient *= this->size[*it];
            
            FractionCellBaseInstance* current = static_cast<FractionCellBaseInstance*>(this->next[quantityIndex]);
            while (current != 0)
            {
                result += current->quantities[quantityIndex];
                current = static_cast<FractionCellBaseInstance*>(current->next[quantityIndex]);
            }
            current = static_cast<FractionCellBaseInstance*>(this->prev[quantityIndex]);
            while (current != 0)
            {
                result += current->quantities[quantityIndex];
                current = static_cast<FractionCellBaseInstance*>(current->prev[quantityIndex]);
            }
        }
        result *= coefficient;
        return result;
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
    
    ////////////////////////
    // Get next/prev/parent functions
    inline FractionsPoolBase* getSpaceCell()
    {
        return static_cast< FractionSpaceBaseInstance* > (this->parentGrid)->parent;
    }
    
    inline FractionCellBase* nextInSpace(unsigned int coordinate)
    {
        FractionsPoolBase *nextCellInSpace = static_cast<FractionsPoolBase*>(getSpaceCell()->next[coordinate]);
        if (!nextCellInSpace) return NULL;
        return &( static_cast<FractionSpaceBaseInstance*> (nextCellInSpace->fractions[FractionIndex])->elements[this->elementIndex] );
    }
    
    inline FractionCellBase* prevInSpace(unsigned int coordinate)
    {
        FractionsPoolBase *nextCellInSpace = static_cast<FractionsPoolBase*>(getSpaceCell()->prev[coordinate]);
        if (!nextCellInSpace) return NULL;
        return &( static_cast<FractionSpaceBaseInstance*> (nextCellInSpace->fractions[FractionIndex])->elements[this->elementIndex] );
    }
    
    inline FractionCellBase* nextInFractionSpace(unsigned int coordinate) { return static_cast<FractionCellBase*>(this->next[coordinate]); }
    inline FractionCellBase* prevInFractionSpace(unsigned int coordinate) { return static_cast<FractionCellBase*>(this->prev[coordinate]); }
    
    /** @brief This function calculates flow from lower cell to higher (flow's projection to axis), so to get outgoing flow in negative direction it should be multiplied by -1
     */
    inline double getFlowInSpace(unsigned int coordinate, FractionCellBaseInstance* neighbor)
    {
        SpaceGridType::GridElementBase* thisSpaceCell = getSpaceCell();
        double l1 = thisSpaceCell->size[coordinate];
        return quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*spaceCoordsDerivatives[coordinate]/l1;
    }
    
    inline double getFlowInFractionSpace(unsigned int coordinate, FractionCellBaseInstance* neighbor)
    {
        double l1 = this->size[coordinate];
        return quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*fractionCoordsDerivatives[coordinate]/l1;
    }
    
private:
    double quantitiesBuffer0[QuantitiesCount];
    double quantitiesBuffer1[QuantitiesCount];
};

#endif // FRACTION_CELL_TEMPLATE
