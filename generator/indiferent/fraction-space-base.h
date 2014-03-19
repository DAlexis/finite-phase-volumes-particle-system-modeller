/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTION_SPACE_BASE_INCLUDED
#define FRACTION_SPACE_BASE_INCLUDED

#include "global-defines.h"
#include "interfaces.h"
#include "fractions-pool-base.h"


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

#endif // FRACTION_SPACE_BASE_INCLUDED
