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
    {
        for (uint i=0; i<AxisCount; i++)
        {
            fractionTopBorderType[i] = BT_WALL; fractionBottomBorderType[i] = BT_WALL;
        }
        for (uint i=0; i<SPACE_COORDS_COUNT; i++)
        {
            spaceTopBorderType[i] = BT_WALL; spaceBottomBorderType[i] = BT_WALL;
        }
    }
    
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
    
    BorderType fractionTopBorderType[AxisCount], fractionBottomBorderType[AxisCount];
    BorderType spaceTopBorderType[SPACE_COORDS_COUNT], spaceBottomBorderType[SPACE_COORDS_COUNT];
    
protected:
    typename Grid<AxisCount, FractionCellType>::GridDescription fractionGridDescription;
};

/// FractionSpaceBase in case AxisCount != 0
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
        // All calculations below means that particles count in this cell is not zero
        if (isNull(quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]))
            return;
        /// @todo Implement here for multi-sign values (like charge)
        /// @todo optimize here?
        // For each quantity
        double flowOutUpward[(FractionSpaceDimension + SpaceDimension) * QuantitiesCount];
        double flowOutDownward[(FractionSpaceDimension + SpaceDimension) * QuantitiesCount];
        double totalFlowOut[QuantitiesCount];
        memset( flowOutUpward, 0, sizeof(double) * (FractionSpaceDimension + SpaceDimension) * QuantitiesCount );
        memset( flowOutDownward, 0, sizeof(double) * (FractionSpaceDimension + SpaceDimension) * QuantitiesCount );
        memset( totalFlowOut, 0, sizeof(double) * QuantitiesCount );
        
        // Particles flows in fraction space
        for (uint coord=0; coord<FractionSpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInFractionSpace(coord);
            FractionCellBaseInstance* prev = prevInFractionSpace(coord);
            double transfer = 0;
            if (next)
                transfer = getConvectiveFlowOutInFractionSpaceUpward(coord, next)*dt;
            else
                transfer = getConvectiveFlowOutThroughFractionTopBorder(coord) * dt;
            
            flowOutUpward[coord] = transfer;
            totalFlowOut[EVERY_FRACTION_COUNT_QUANTITY_INDEX] += transfer;
            
            if (prev)
                transfer = getConvectiveFlowOutInFractionSpaceDownward(coord, prev)*dt;
            else
                transfer = -getConvectiveFlowOutThroughFractionBottomBorder(coord) * dt;
            
            flowOutDownward[coord] = transfer;
            totalFlowOut[EVERY_FRACTION_COUNT_QUANTITY_INDEX] += transfer;
        }
        
        // Particles flows in coordinate space
        for (uint coord=0; coord<SpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInSpace(coord);
            FractionCellBaseInstance* prev = prevInSpace(coord);
            double transfer = 0;
            if (next)
                transfer = (getConvectiveFlowOutInSpaceUpward(coord, next)
                    + getDiffusionFlowOutInSpace(coord, EVERY_FRACTION_COUNT_QUANTITY_INDEX, next)) * dt;
            else
                transfer = getConvectiveFlowOutThroughCoordinateTopBorder(coord) * dt;
            
            flowOutUpward[FractionSpaceDimension + coord] = transfer;
            totalFlowOut[EVERY_FRACTION_COUNT_QUANTITY_INDEX] += transfer;
            
            if (prev)
                transfer = (getConvectiveFlowOutInSpaceDownward(coord, prev)
                    + getDiffusionFlowOutInSpace(coord, EVERY_FRACTION_COUNT_QUANTITY_INDEX, prev)) * dt;
            else
                transfer = -getConvectiveFlowOutThroughCoordinateBottomBorder(coord) * dt;
            
            flowOutDownward[FractionSpaceDimension + coord] = transfer;
            totalFlowOut[EVERY_FRACTION_COUNT_QUANTITY_INDEX] += transfer;
        }
        /////////////////////////
        // Counting diffusion flows out for other quantities
        // Cache for frequently used q[i]/n
        double quantityOverParticlesCount[QuantitiesCount];
        /// We know that particles count is not null (see function beginning)
        for (unsigned int quantity=0; quantity<QuantitiesCount; quantity++)
            quantityOverParticlesCount[quantity] = quantities[quantity] / quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX];
        
        // In fraction space
        for (uint coord=0; coord<FractionSpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInFractionSpace(coord);
            FractionCellBaseInstance* prev = prevInFractionSpace(coord);
            double transfer = 0;
            // Upward
            if (next) {
                for (uint quantity=EVERY_FRACTION_COUNT_QUANTITY_INDEX+1; quantity<QuantitiesCount; quantity++) {
                    transfer = flowOutUpward[coord] * quantityOverParticlesCount[quantity]; // Flows out with particles
                    flowOutUpward[(FractionSpaceDimension + SpaceDimension)*quantity + coord] = transfer;
                    totalFlowOut[quantity] += transfer;
                }
            } else {
                for (uint quantity=EVERY_FRACTION_COUNT_QUANTITY_INDEX+1; quantity<QuantitiesCount; quantity++) {
                    transfer = flowOutUpward[coord] * quantityOverParticlesCount[quantity]; // Flows out with particles
                    flowOutUpward[(FractionSpaceDimension + SpaceDimension)*quantity + coord] = transfer;
                    totalFlowOut[quantity] += transfer;
                }
            }
            
            // Downward
            if (prev) {
                for (uint quantity=EVERY_FRACTION_COUNT_QUANTITY_INDEX+1; quantity<QuantitiesCount; quantity++) {
                    transfer = flowOutDownward[coord] * quantityOverParticlesCount[quantity]; // Flows out with particles
                    flowOutDownward[(FractionSpaceDimension + SpaceDimension)*quantity + coord] = transfer;
                    totalFlowOut[quantity] += transfer;
                }
            } else {
                for (uint quantity=EVERY_FRACTION_COUNT_QUANTITY_INDEX+1; quantity<QuantitiesCount; quantity++) {
                    transfer = flowOutDownward[coord] * quantityOverParticlesCount[quantity]; // Flows out with particles
                    // No diffusion out of space
                    flowOutDownward[(FractionSpaceDimension + SpaceDimension)*quantity + coord] = transfer;
                    totalFlowOut[quantity] += transfer;
                }
            }
        }
        
        // In coordinate space
        for (uint coord=0; coord<SpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInSpace(coord);
            FractionCellBaseInstance* prev = prevInSpace(coord);
            double transfer = 0;
            // Upward
            if (next) {
                for (uint quantity=EVERY_FRACTION_COUNT_QUANTITY_INDEX+1; quantity<QuantitiesCount; quantity++) {
                    transfer = flowOutUpward[FractionSpaceDimension + coord] * quantityOverParticlesCount[quantity] // Flows out with particles
                        + getDiffusionFlowOutInSpace(coord, quantity, next) * dt; // Flows out with diffusion
                    flowOutUpward[(FractionSpaceDimension + SpaceDimension)*quantity + FractionSpaceDimension + coord] = transfer;
                    totalFlowOut[quantity] += transfer;
                }
            } else {
                for (uint quantity=EVERY_FRACTION_COUNT_QUANTITY_INDEX+1; quantity<QuantitiesCount; quantity++) {
                    transfer = flowOutUpward[FractionSpaceDimension + coord] * quantityOverParticlesCount[quantity]; // Flows out with particles
                    // No diffusion out of space
                    flowOutUpward[(FractionSpaceDimension + SpaceDimension)*quantity + FractionSpaceDimension + coord] = transfer;
                    totalFlowOut[quantity] += transfer;
                }
            }
            
            // Downward
            if (prev) {
                for (uint quantity=EVERY_FRACTION_COUNT_QUANTITY_INDEX+1; quantity<QuantitiesCount; quantity++) {
                    transfer = flowOutDownward[FractionSpaceDimension + coord] * quantityOverParticlesCount[quantity] // Flows out with particles
                        + getDiffusionFlowOutInSpace(coord, quantity, prev) * dt; // Flows out with diffusion
                    flowOutDownward[(FractionSpaceDimension + SpaceDimension)*quantity + FractionSpaceDimension + coord] = transfer;
                    totalFlowOut[quantity] += transfer;
                }
            } else {
                for (uint quantity=EVERY_FRACTION_COUNT_QUANTITY_INDEX+1; quantity<QuantitiesCount; quantity++) {
                    transfer = flowOutDownward[FractionSpaceDimension + coord] * quantityOverParticlesCount[quantity]; // Flows out with particles
                    // No diffusion out of space
                    flowOutDownward[(FractionSpaceDimension + SpaceDimension)*quantity + FractionSpaceDimension + coord] = transfer;
                    totalFlowOut[quantity] += transfer;
                }
            }
        }
        
        // Now we know all flows, but we dont know is it enough each quantity in cell?
        for (uint quantity=EVERY_FRACTION_COUNT_QUANTITY_INDEX; quantity<QuantitiesCount; quantity++)
        {
            if (totalFlowOut[quantity] > quantities[quantity])
            {
                double renormCoefficient = quantities[quantity] / totalFlowOut[quantity];
                for (uint i=0; i < FractionSpaceDimension + SpaceDimension; i++)
                {
                    flowOutDownward[(FractionSpaceDimension + SpaceDimension)*quantity + i] *= renormCoefficient;
                    flowOutUpward[(FractionSpaceDimension + SpaceDimension)*quantity + i] *= renormCoefficient;
                }
                totalFlowOut[quantity] = quantities[quantity];
            }
        }
        
        // Now we sure that counted flows out are correct
        // We can remove and add values now
        // Removing values
        for (uint quantity=0; quantity<QuantitiesCount; quantity++)
            nextStepQuantities[quantity] -= totalFlowOut[quantity];
        // Adding values in fraction space
        for (uint coord=0; coord<FractionSpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInFractionSpace(coord);
            FractionCellBaseInstance* prev = prevInFractionSpace(coord);
            // Upward
            if (next)
                for (uint quantity=0; quantity<QuantitiesCount; quantity++)
                    next->nextStepQuantities[quantity] += flowOutUpward[(FractionSpaceDimension + SpaceDimension)*quantity + coord];
            // Downward
            if (prev)
                for (uint quantity=0; quantity<QuantitiesCount; quantity++)
                    prev->nextStepQuantities[quantity] += flowOutDownward[(FractionSpaceDimension + SpaceDimension)*quantity + coord];
        }
        
        // Adding values in coordinate space
        for (uint coord=0; coord<SpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInSpace(coord);
            FractionCellBaseInstance* prev = prevInSpace(coord);
            // Upward
            if (next)
                for (uint quantity=0; quantity<QuantitiesCount; quantity++)
                    next->nextStepQuantities[quantity] += flowOutUpward[(FractionSpaceDimension + SpaceDimension)*quantity + FractionSpaceDimension + coord];
            // Downward
            if (prev)
                for (uint quantity=0; quantity<QuantitiesCount; quantity++)
                    prev->nextStepQuantities[quantity] += flowOutDownward[(FractionSpaceDimension + SpaceDimension)*quantity + FractionSpaceDimension + coord];
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
    
    virtual double getSpaceDiffusionCoefficient(uint quantity, uint axisIndex) = 0;
    
private:
    double quantitiesBuffer0[QuantitiesCount];
    double quantitiesBuffer1[QuantitiesCount];
    
    ///////////////////
    // Convective flows couning

    /// This function calculates flow from lower cell to higher (flow's projection to axis), so to get OUTGOING flow in negative direction it should be multiplied by -1
    inline double getConvectiveFlowInSpace(unsigned int coordinate, FractionCellBaseInstance* neighbor)
    {
        SpaceGridType::GridElementBase* thisSpaceCell = getSpaceCell();
        double l1 = thisSpaceCell->size[coordinate];
        return quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*spaceCoordsDerivatives[coordinate]/l1;
    }
    
    /// This function calculates flow from lower cell to higher (flow's projection to axis), so to get OUTGOING flow in negative direction it should be multiplied by -1
    inline double getConvectiveFlowInFractionSpace(unsigned int coordinate, FractionCellBaseInstance* neighbor)
    {
        double l1 = this->size[coordinate];
        return quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*fractionCoordsDerivatives[coordinate]/l1;
    }
    
    
    inline double getConvectiveFlowOutInSpaceUpward(unsigned int coordinate, FractionCellBaseInstance* neighbor)
    {
        double result = getConvectiveFlowInSpace(coordinate, neighbor);
        if (result > 0.0)
            return result;
        else
            return 0.0;
    }
    
    inline double getConvectiveFlowOutInFractionSpaceUpward(unsigned int coordinate, FractionCellBaseInstance* neighbor)
    {
        double result = getConvectiveFlowInFractionSpace(coordinate, neighbor);
        if (result > 0.0)
            return result;
        else
            return 0.0;
    }
    
    inline double getConvectiveFlowOutInSpaceDownward(unsigned int coordinate, FractionCellBaseInstance* neighbor)
    {
        double result = getConvectiveFlowInSpace(coordinate, neighbor);
        if (result < 0.0)
            return -result;
        else
            return 0.0;
    }
    
    inline double getConvectiveFlowOutInFractionSpaceDownward(unsigned int coordinate, FractionCellBaseInstance* neighbor)
    {
        double result = getConvectiveFlowInFractionSpace(coordinate, neighbor);
        if (result < 0.0)
            return -result;
        else
            return 0.0;
    }
    
    /// This function calculates diffusion flow from this to neighbor, so no multiplying by -1 needed to get OUTGOING flow
    inline double getDiffusionFlowOutInSpace(unsigned int coordinate, unsigned int quantity, FractionCellBaseInstance* neighbor)
    {
        SpaceGridType::GridElementBase* thisSpaceCell = getSpaceCell();
        SpaceGridType::GridElementBase* neighborSpaceCell = neighbor->getSpaceCell();
        double l1 = thisSpaceCell->size[coordinate];
        double l2 = neighborSpaceCell->size[coordinate];
        double result = (quantities[quantity]/l1 - neighbor->quantities[quantity]/l2) / (l1+l2)*2 * getSpaceDiffusionCoefficient(quantity, coordinate);
        if (result > 0.0)
            return result;
        else
            return 0.0;
    }
    
    /// @todo May be optimexed checking of velocity sign. Now sign is sign of flow projection.
    inline double getConvectiveFlowOutThroughCoordinateTopBorder(uint coordinate)
    {
        switch (static_cast<FractionSpaceBaseInstance*>(this->parentGrid)->spaceTopBorderType[coordinate])
        {
            case BT_OPEN: {
                // Case of open border
                SpaceGridType::GridElementBase* thisSpaceCell = getSpaceCell();
                double l1 = thisSpaceCell->size[coordinate];
                if (spaceCoordsDerivatives[coordinate] > 0)
                    return quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*spaceCoordsDerivatives[coordinate]/l1;
                else
                    return 0.0;
            }
            default:
            case BT_WALL:
                return 0.0;
        }
    }
    
    inline double getConvectiveFlowOutThroughCoordinateBottomBorder(uint coordinate)
    {
        switch (static_cast<FractionSpaceBaseInstance*>(this->parentGrid)->spaceBottomBorderType[coordinate])
        {
            case BT_OPEN: {
                // Case of open border
                SpaceGridType::GridElementBase* thisSpaceCell = getSpaceCell();
                double l1 = thisSpaceCell->size[coordinate];
                if (spaceCoordsDerivatives[coordinate] < 0)
                    return quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*spaceCoordsDerivatives[coordinate]/l1;
                else
                    return 0.0;
            }
            default:
            case BT_WALL:
                return 0.0;
        }
    }
    
    inline double getConvectiveFlowOutThroughFractionTopBorder(uint coordinate)
    {
        switch (static_cast<FractionSpaceBaseInstance*>(this->parentGrid)->fractionTopBorderType[coordinate])
        {
            case BT_OPEN: {
                // Case of open border
                double l1 = this->size[coordinate];
                if (fractionCoordsDerivatives[coordinate] > 0)
                    return quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*fractionCoordsDerivatives[coordinate]/l1;
                else
                    return 0.0;
            }
            default:
            case BT_WALL:
                return 0.0;
        }
    }
    
    inline double getConvectiveFlowOutThroughFractionBottomBorder(uint coordinate)
    {
        switch (static_cast<FractionSpaceBaseInstance*>(this->parentGrid)->fractionBottomBorderType[coordinate])
        {
            case BT_OPEN: {
                // Case of open border
                double l1 = this->size[coordinate];
                if (fractionCoordsDerivatives[coordinate] < 0)
                    return quantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*fractionCoordsDerivatives[coordinate]/l1;
                else
                    return 0.0;
            }
            default:
            case BT_WALL:
                return 0.0;
        }
    }
    
};

#endif // FRACTION_CELL_TEMPLATE
