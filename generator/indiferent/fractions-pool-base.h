/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef FRACTIONS_POOL_TEMPLATE
#define FRACTIONS_POOL_TEMPLATE

#include "global-defines.h"
#include "interfaces.h"
#include <string.h>

/// @todo Remove this magic constant
#define MAX_EXTENSIVE_QUANTITIES_COUNT  20

template<int SpaceDimension, int FractionsCount>
class FractionsPoolBase : public GridElementBase<SpaceDimension>, public IFractionsPool
{
public:
    FractionsPoolBase() :
        averagingBlockSize(0),
        quantitiesAveragingBufferSize(0),
        averagingBlockElements(0),
        quantitiesAveragingBuffer(0)
    {
        for (unsigned int i=0; i<FractionsCount; i++)
            fractions[i] = NULL;
    }
    
    virtual ~FractionsPoolBase()
    {
        if (averagingBlockElements) delete[] averagingBlockElements;
        if (quantitiesAveragingBuffer) delete[] quantitiesAveragingBuffer;
        for (unsigned int i=0; i<FractionsCount; i++)
            if (fractions[i]) delete fractions[i];
    }
    
    void initQuantities()
    {
        for (unsigned int i=0; i<FractionsCount; i++)
            fractions[i]->initQuantities();
    }

    void calculateEvolution(double dt)
    {
        for (unsigned int i=0; i<FractionsCount; i++) {
            fractions[i]->calculateSourceEvolution(dt);
            fractions[i]->calculateFlowsEvolution(dt);
        }
    }
    
    void addDelta()
    {
        for (unsigned int i=0; i<FractionsCount; i++)
            fractions[i]->addDelta();
    }
    
    IFractionSpace* getFraction(unsigned int fractionId) { return fractions[fractionId]; }
    double getVolume() { return this->volume; }
    
    IFractionSpace *fractions[FractionsCount];
    
    // Works now for fraction #0
    void averageWithNext()
    {
        if (averagingBlockElements == 0) {
            initAveragingBlock();
        }
        
        memset(quantitiesAveragingBuffer, 0, sizeof(double)*quantitiesAveragingBufferSize);
        
        for (int i=0; i<averagingBlockSize; i++)
            averagingBlockElements[i]->fractions[0]->storeDataToAveragingBuffer(quantitiesAveragingBuffer);
            
        for (int i=0; i<quantitiesAveragingBufferSize; i++)
            quantitiesAveragingBuffer[i] /= averagingBlockSize;
        
        for (int i=0; i<averagingBlockSize; i++)
            averagingBlockElements[i]->fractions[0]->restoreDataFromAveragingBuffer(quantitiesAveragingBuffer);
    }
    
    void averageWithNeighbours()
    {
        fractions[0]->averageWithNeighbours();
    }
    
protected:
private:
    void initAveragingBlock()
    {
        averagingBlockSize = 1;
        for (int i=0; i<SpaceDimension; i++) averagingBlockSize *= 2;
        averagingBlockElements = new FractionsPoolBase*[averagingBlockSize];
        quantitiesAveragingBufferSize = MAX_EXTENSIVE_QUANTITIES_COUNT;
        quantitiesAveragingBuffer = new double[quantitiesAveragingBufferSize];
        // Filling averagingBlockElements
        for (int i=0; i<averagingBlockSize; i++) {
            FractionsPoolBase* current = this;
            for (int axisIndex=0; axisIndex<SpaceDimension; axisIndex++)
            {
                /* This is not magic, i is from 0b000 ~ (this, this, this) to 0b111 ~ (next, next, next).
                 * Here 1 means next for this dimension and 0 means do not move in this dimension */
                if ( i & (1 << axisIndex) ) current = static_cast<FractionsPoolBase*>( current->next[axisIndex] );
            }
            averagingBlockElements[i] = current;
        }
    }
    
    int averagingBlockSize;
    int quantitiesAveragingBufferSize;
    FractionsPoolBase** averagingBlockElements;
    double* quantitiesAveragingBuffer;
};

//class FractionsPoolBase : public FractionsPoolBaseTmp<SPACE_COORDS_COUNT> {};

#endif // FRACTIONS_POOL_TEMPLATE
