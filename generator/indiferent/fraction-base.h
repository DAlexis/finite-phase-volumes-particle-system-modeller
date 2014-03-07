/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTION_CELL_TEMPLATE
#define FRACTION_CELL_TEMPLATE

#include "global-defines.h"
#include "interfaces.h"
#include "fractions-pool-base.h"

#include <string.h>

/// This is a part of FractionSpaceBase that is indifferent to case FractionSpaceDimension == 0
template <int SpaceDimension,
    int FractionSpaceDimension,
    int FractionsCount,
    class FractionCellType>
class FractionSpaceBaseIncomplete : public Grid<FractionSpaceDimension, FractionCellType>, public IFractionSpace
{
public:
    typedef Grid<FractionSpaceDimension, FractionCellType> GridInstance;
    typedef FractionsPoolBase<SpaceDimension, FractionsCount> FractionsPoolBaseInstance;
    
    FractionSpaceBaseIncomplete(FractionsPoolBaseInstance* parentFractionsPool) :
        parent(parentFractionsPool)
    {
        for (uint i=0; i<FractionSpaceDimension; i++)
        {
            fractionTopBorderType[i] = BT_WALL; fractionBottomBorderType[i] = BT_WALL;
        }
        for (uint i=0; i<SpaceDimension; i++)
        {
            spaceTopBorderType[i] = BT_WALL; spaceBottomBorderType[i] = BT_WALL;
        }
    }
    
    virtual ~FractionSpaceBaseIncomplete() { }
    
    void initQuantities()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->initQuantities();
        
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateIntensiveQuantities();
    }
        
    void calculateFlowsEvolution(double dt)
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateIntensiveQuantities();
        
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateDerivatives();
        
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateFlowsEvolution(dt);
        
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateQuantitiesDiffusion(dt);
    }
    
    void calculateSourceEvolution(double dt)
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->calculateSourceEvolution(dt);
    }
    
    void addDelta()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            static_cast<IFractionCell*>( &(this->elements[i]))->addDelta();
    }
    
    IFractionCell* getCell(const double* coords)
    {
        return static_cast<IFractionCell*> (this->accessElement_d(coords));
    }
    
    double getQuantitiesSum(unsigned int quantityIndex)
    {
        double sum=0;
        for (unsigned int i=0; i<this->elementsCount; i++)
            sum += this->elements[i].extensiveQuantities[quantityIndex];
        return sum;
    }
    
    //const Axis* getAxisDescription(unsigned int axis);
    
    FractionsPoolBaseInstance* parent;
    
    BorderType fractionTopBorderType[FractionSpaceDimension], fractionBottomBorderType[FractionSpaceDimension];
    BorderType spaceTopBorderType[SpaceDimension], spaceBottomBorderType[SpaceDimension];
    
protected:
    typename Grid<FractionSpaceDimension, FractionCellType>::GridDescription fractionGridDescription;
};

/// FractionSpaceBase in case FractionSpaceDimension != 0
template <int SpaceDimension,
    int FractionSpaceDimension,
    int FractionsCount,
    class FractionCellType>
class FractionSpaceBase : public FractionSpaceBaseIncomplete<SpaceDimension, FractionSpaceDimension, FractionsCount, FractionCellType>
{
public:
    typedef FractionsPoolBase<SpaceDimension, FractionsCount> FractionsPoolBaseInstance;
    FractionSpaceBase(FractionsPoolBaseInstance* parentFractionsPool) :
        FractionSpaceBaseIncomplete<SpaceDimension, FractionSpaceDimension, FractionsCount, FractionCellType>(parentFractionsPool)
    {}
    
    virtual ~FractionSpaceBase() {}
    typedef FractionSpaceBase<SpaceDimension, FractionSpaceDimension, FractionsCount, FractionCellType> FractionSpaceBaseInstance;
    
    const Axis* getAxisDescription(unsigned int axis) { return &(this->gridDescription->axis[axis]); }
};

// Template specification when fraction axis count is 0
template <int SpaceDimension,
    int FractionsCount,
    class FractionCellType>
class FractionSpaceBase<SpaceDimension, 0,  FractionsCount, FractionCellType> : public FractionSpaceBaseIncomplete<SpaceDimension, 0, FractionsCount, FractionCellType>
{
public:
    typedef FractionsPoolBase<SpaceDimension, FractionsCount> FractionsPoolBaseInstance;
    FractionSpaceBase(FractionsPoolBaseInstance* parentFractionsPool) :
        FractionSpaceBaseIncomplete<SpaceDimension, 0, FractionsCount, FractionCellType>(parentFractionsPool)
    {}
    
    virtual ~FractionSpaceBase() {}
    typedef FractionSpaceBase<SpaceDimension, 0, FractionsCount, FractionCellType> FractionSpaceBaseInstance;
    
    /// This function is not ever called and is here only to prevent compilation errors
    const Axis* getAxisDescription(unsigned int axis) { return NULL; }
};

template <int FractionIndex,
          int FractionsCount,
          int SpaceDimension,
          int FractionSpaceDimension,
          int QuantitiesCount,
          int SecondaryQuantitiesCount,
          class FractionCellType>
class FractionCellBase : public GridElementBase<FractionSpaceDimension>, public IFractionCell
{
public:
    typedef FractionCellBase FractionCellBaseInstance;
    typedef FractionSpaceBase<SpaceDimension, FractionSpaceDimension, FractionsCount, FractionCellType> FractionSpaceBaseInstance;
    typedef Grid<SpaceDimension, FractionCellType> GridInstance;
    typedef FractionsPoolBase<SpaceDimension, FractionsCount> FractionsPoolBaseInstance;
    
    FractionCellBase()
    {
        for (unsigned int i=0; i<QuantitiesCount; i++) {
            extensiveQuantities[i] = 0.0;
            extensiveQuantitiesDelta[i] = 0.0;
        }
        for (unsigned int i=0; i<SecondaryQuantitiesCount; i++)
            intensiveQuantities[i] = 0.0;
    }
    
    virtual ~FractionCellBase() { }
    
    void addDelta()
    {
        for (unsigned int i=0; i<QuantitiesCount; i++)
        {
            extensiveQuantities[i] += extensiveQuantitiesDelta[i];
            extensiveQuantitiesDelta[i] = 0.0;
        }
    }
    
    void calculateFlowsEvolution(double dt)
    {
        // All calculations below means that particles count in this cell is not zero
        if (isNull(extensiveQuantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]))
            return;
        /// @todo Implement here for multi-sign values (like charge)
        /// @todo optimize here?
        // For each quantity
        double flowOutUpward[FractionSpaceDimension + SpaceDimension];
        double flowOutDownward[FractionSpaceDimension + SpaceDimension];
        double totalFlowOut = 0;
        memset( flowOutUpward, 0, sizeof(double) * (FractionSpaceDimension + SpaceDimension) );
        memset( flowOutDownward, 0, sizeof(double) * (FractionSpaceDimension + SpaceDimension) );
        
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
            totalFlowOut += transfer;
            
            if (prev)
                transfer = getConvectiveFlowOutInFractionSpaceDownward(coord, prev)*dt;
            else
                transfer = -getConvectiveFlowOutThroughFractionBottomBorder(coord) * dt;
            
            flowOutDownward[coord] = transfer;
            totalFlowOut += transfer;
        }
        
        // Particles flows in coordinate space
        for (uint coord=0; coord<SpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInSpace(coord);
            FractionCellBaseInstance* prev = prevInSpace(coord);
            double transfer = 0;
            if (next)
                transfer = (getConvectiveFlowOutInSpaceUpward(coord, next)
                    + getParticlesCountDiffusionFlowOutInSpace(coord, next)) * dt;
            else
                transfer = getConvectiveFlowOutThroughCoordinateTopBorder(coord) * dt;
            
            flowOutUpward[FractionSpaceDimension + coord] = transfer;
            totalFlowOut += transfer;
            
            if (prev)
                transfer = (getConvectiveFlowOutInSpaceDownward(coord, prev)
                    + getParticlesCountDiffusionFlowOutInSpace(coord, prev)) * dt;
            else
                transfer = -getConvectiveFlowOutThroughCoordinateBottomBorder(coord) * dt;
            
            flowOutDownward[FractionSpaceDimension + coord] = transfer;
            totalFlowOut += transfer;
        }
        
        // Cache for frequently used q[i]/n
        double quantityOverParticlesCount[QuantitiesCount];
        /// We know that particles count is not null (see function beginning)
        for (unsigned int quantity=0; quantity<QuantitiesCount; quantity++)
            quantityOverParticlesCount[quantity] = extensiveQuantities[quantity] / extensiveQuantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX];
        
        // Check if we trying to remove more particles that we have
        if (totalFlowOut > extensiveQuantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX])
        {
            double outTransferRenormCoefficient = extensiveQuantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX] / totalFlowOut;
            for (uint coord=0; coord < FractionSpaceDimension + SpaceDimension; coord++)
            {
                flowOutUpward[coord] *= outTransferRenormCoefficient;
                flowOutDownward[coord] *= outTransferRenormCoefficient;
            }
            totalFlowOut = extensiveQuantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX];
        }
        
        // Removing quantities
        for (uint quantity=EVERY_FRACTION_COUNT_QUANTITY_INDEX; quantity<QuantitiesCount; quantity++)
        {
            extensiveQuantitiesDelta[quantity] -= totalFlowOut * quantityOverParticlesCount[quantity];
        }
        
        // Adding quantities to neighbors
        for (uint coord=0; coord<FractionSpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInFractionSpace(coord);
            FractionCellBaseInstance* prev = prevInFractionSpace(coord);
            // Upward
            if (next)
                for (uint quantity=0; quantity<QuantitiesCount; quantity++)
                    next->extensiveQuantitiesDelta[quantity] += flowOutUpward[coord] * quantityOverParticlesCount[quantity];
            // Downward
            if (prev)
                for (uint quantity=0; quantity<QuantitiesCount; quantity++)
                    prev->extensiveQuantitiesDelta[quantity] += flowOutDownward[coord] * quantityOverParticlesCount[quantity];
        }
        
        // Adding values in coordinate space
        for (uint coord=0; coord<SpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInSpace(coord);
            FractionCellBaseInstance* prev = prevInSpace(coord);
            // Upward
            if (next)
                for (uint quantity=0; quantity<QuantitiesCount; quantity++)
                    next->extensiveQuantitiesDelta[quantity] += flowOutUpward[FractionSpaceDimension + coord] * quantityOverParticlesCount[quantity];
            // Downward
            if (prev)
                for (uint quantity=0; quantity<QuantitiesCount; quantity++)
                    prev->extensiveQuantitiesDelta[quantity] += flowOutDownward[FractionSpaceDimension + coord] * quantityOverParticlesCount[quantity];
        }
        
        // Now all transfer coupled with particles transfer is done
    }
    
    void calculateQuantitiesDiffusion(double dt)
    {
        for (uint coord=0; coord<SpaceDimension; coord++)
        {
            FractionCellBaseInstance* next = nextInSpace(coord);
            // Upward
            if (next) {
                for (uint quantity=EVERY_FRACTION_COUNT_QUANTITY_INDEX+1; quantity<QuantitiesCount; quantity++) {
                    double diffusionFromThis = getDiffusionFlowInSpace(coord, quantity, next) * dt;
                    extensiveQuantitiesDelta[quantity] -= diffusionFromThis;
                    next->extensiveQuantitiesDelta[quantity] += diffusionFromThis;
                }
            }
        }
    }
    
    double getQuantitiesDensity(unsigned int index)
    {
        return extensiveQuantities[index] / this->volume;
    }
    
    double getQuantitiesDensityConvolution(unsigned int quantityIndex, const std::vector<unsigned int>& convoluteBy)
    {
        double result = extensiveQuantities[quantityIndex];
        double coefficient = 1 / this->volume;
        for (auto it=convoluteBy.begin(); it != convoluteBy.end(); it++)
        {
            // Convolution by one dimension
            coefficient *= this->size[*it];
            
            FractionCellBaseInstance* current = static_cast<FractionCellBaseInstance*>(this->next[quantityIndex]);
            while (current != 0)
            {
                result += current->extensiveQuantities[quantityIndex];
                current = static_cast<FractionCellBaseInstance*>(current->next[quantityIndex]);
            }
            current = static_cast<FractionCellBaseInstance*>(this->prev[quantityIndex]);
            while (current != 0)
            {
                result += current->extensiveQuantities[quantityIndex];
                current = static_cast<FractionCellBaseInstance*>(current->prev[quantityIndex]);
            }
        }
        result *= coefficient;
        return result;
    }
    
    double getIntensiveQuantity(unsigned int intensiveQuantityIndex) { return intensiveQuantities[intensiveQuantityIndex]; }
    
    double extensiveQuantities[QuantitiesCount];
    double extensiveQuantitiesDelta[QuantitiesCount];
    
    double intensiveQuantities[SecondaryQuantitiesCount];
    
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
    inline FractionsPoolBaseInstance* getSpaceCell()
    {
        return static_cast< FractionSpaceBaseInstance* > (this->parentGrid)->parent;
    }
    
    inline FractionCellBase* nextInSpace(unsigned int coordinate)
    {
        FractionsPoolBaseInstance *nextCellInSpace = static_cast<FractionsPoolBaseInstance*>(getSpaceCell()->next[coordinate]);
        if (!nextCellInSpace) return NULL;
        return &( static_cast<FractionSpaceBaseInstance*> (nextCellInSpace->fractions[FractionIndex])->elements[this->elementIndex] );
    }
    
    inline FractionCellBase* prevInSpace(unsigned int coordinate)
    {
        FractionsPoolBaseInstance *nextCellInSpace = static_cast<FractionsPoolBaseInstance*>(getSpaceCell()->prev[coordinate]);
        if (!nextCellInSpace) return NULL;
        return &( static_cast<FractionSpaceBaseInstance*> (nextCellInSpace->fractions[FractionIndex])->elements[this->elementIndex] );
    }
    
    inline double interpolateIntensiveQunatityOnSpaceCellsContact(unsigned int quantityId, FractionCellBaseInstance* neighbor, unsigned int spaceCoordinate)
    {
        double l1 = getSpaceCell()->size[spaceCoordinate];
        double l2 = getSpaceCell()->size[spaceCoordinate];
        return (intensiveQuantities[quantityId]*l1 + neighbor->intensiveQuantities[quantityId]*l2) / (l1+l2);
    }
    
    inline FractionCellBase* nextInFractionSpace(unsigned int coordinate) { return static_cast<FractionCellBase*>(this->next[coordinate]); }
    inline FractionCellBase* prevInFractionSpace(unsigned int coordinate) { return static_cast<FractionCellBase*>(this->prev[coordinate]); }
    
    virtual double getSpaceDiffusionCoefficient(uint quantity, uint axisIndex) = 0;
    
private:
    
    ///////////////////
    // Convective flows couning

    /// This function calculates flow from lower cell to higher (flow's projection to axis), so to get OUTGOING flow in negative direction it should be multiplied by -1
    inline double getConvectiveFlowInSpace(unsigned int coordinate, FractionCellBaseInstance* neighbor)
    {
        GridElementBase<SpaceDimension>* thisSpaceCell = getSpaceCell();
        double l1 = thisSpaceCell->size[coordinate];
        return extensiveQuantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*spaceCoordsDerivatives[coordinate]/l1;
    }
    
    /// This function calculates flow from lower cell to higher (flow's projection to axis), so to get OUTGOING flow in negative direction it should be multiplied by -1
    inline double getConvectiveFlowInFractionSpace(unsigned int coordinate, FractionCellBaseInstance* neighbor)
    {
        double l1 = this->size[coordinate];
        return extensiveQuantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*fractionCoordsDerivatives[coordinate]/l1;
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
    
    ///////////////////
    // Diffusion flows couning
    inline double getDiffusionFlowInSpace(unsigned int coordinate, unsigned int quantity, FractionCellBaseInstance* neighbor)
    {
        GridElementBase<SpaceDimension>* thisSpaceCell = getSpaceCell();
        GridElementBase<SpaceDimension>* neighborSpaceCell = neighbor->getSpaceCell();
        double l1 = thisSpaceCell->size[coordinate];
        double l2 = neighborSpaceCell->size[coordinate];
        return (extensiveQuantities[quantity]/l1 - neighbor->extensiveQuantities[quantity]/l2) / (l1+l2)*2 * getSpaceDiffusionCoefficient(quantity, coordinate);
    }
    /// This function calculates diffusion flow from this to neighbor, so no multiplying by -1 needed to get OUTGOING flow
    inline double getParticlesCountDiffusionFlowOutInSpace(unsigned int coordinate, FractionCellBaseInstance* neighbor)
    {
        double result = getDiffusionFlowInSpace(coordinate, EVERY_FRACTION_COUNT_QUANTITY_INDEX, neighbor);
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
                GridElementBase<SpaceDimension>* thisSpaceCell = getSpaceCell();
                double l1 = thisSpaceCell->size[coordinate];
                if (spaceCoordsDerivatives[coordinate] > 0)
                    return extensiveQuantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*spaceCoordsDerivatives[coordinate]/l1;
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
                GridElementBase<SpaceDimension>* thisSpaceCell = getSpaceCell();
                double l1 = thisSpaceCell->size[coordinate];
                if (spaceCoordsDerivatives[coordinate] < 0)
                    return extensiveQuantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*spaceCoordsDerivatives[coordinate]/l1;
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
                    return extensiveQuantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*fractionCoordsDerivatives[coordinate]/l1;
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
                    return extensiveQuantities[EVERY_FRACTION_COUNT_QUANTITY_INDEX]*fractionCoordsDerivatives[coordinate]/l1;
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
