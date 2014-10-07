/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef SPACE_BASE_H_INCLUDED
#define SPACE_BASE_H_INCLUDED

#include "global-defines.h"
#include "interfaces.h"
#include "threads-pool.h"
#include <mutex>
#include <functional>
#include <memory>
#include <vector>

template<int SpaceDimension, class FractionPoolType>
class SpaceBase : public Grid<SpaceDimension, FractionPoolType>, public ISpace
{
public:
    typedef FractionPoolType FractionsPoolInstance;
    typedef Grid<SpaceDimension, FractionPoolType> SpaceGridInstance;
    typedef GridElementBase<SpaceDimension> SpaceGridElementInstance;
    
    SpaceBase() :
        m_threadsCount(THREADS_COUNT),
        cellsAreIndexed(false)
    {
    }
    
    virtual ~SpaceBase() { }
    
    void turnOnAveraging(unsigned int fractionId)
    {
        fractionsThatNeedAveraging.push_back(fractionId);
    }
    
    void initQuantities()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            this->elements[i].initQuantities();
    }
    
    void indexateCells()
    {
        if (cellsAreIndexed)
            return;
        for (size_t i=0; i<this->elementsCount; i++)
            this->elements[i].addElements(allCells);
        cellsAreIndexed = true;
        
        initThreads();
    }
    
    void calculateEvolution(double dt)
    {
        if (m_threadsCount == 1) {
            iterateByIndexedCells(0, allCells.size(), dt, 0);
            return;
        }
        threadsPool.unlockThreads(dt);
        threadsPool.wait();
        
    }
    
    void addDelta()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            this->elements[i].addDelta();
    }
    
    void setParent(void* model) { parent = model; }
    
    IFractionsPool* getCellByIndex(const unsigned int index) { return &(this->elements[index]); }
    
    void initThreads()
    {
        m_threadsCount = THREADS_COUNT;
        
        for (unsigned int i=0; i<m_threadsCount; i++)
        {
            threadsPool.addThread(
                std::bind(
                    &SpaceBase::iterateByIndexedCells,
                    this,
                    allCells.size() * i / m_threadsCount,
                    allCells.size() * (i+1) / m_threadsCount,
                    std::placeholders::_1,
                    i
                    )
                );
        }
    }
    
    void stopThreads()
    {
        threadsPool.stopThreads();
    }
    
    void averageWithNeighbours()
    {
        for (uint i=0; i < this->elementsCount; i++)
            for (auto it=fractionsThatNeedAveraging.begin(); it!=fractionsThatNeedAveraging.end(); it++)
                this->elements[i].averageWithNeighbours(*it);
    }
    
    // Realisation of ISapce interface
    IFractionsPool* getCell_d(const double* coords) { return this->accessElement_d(coords); }
    IFractionsPool* getCell_ui(const uint* coords) { return this->accessElement_ui(coords); }
    const Axis* getAxisDescription(unsigned int axis) { return &(this->gridDescription->axis[axis]); }
    
    typename SpaceGridInstance::GridDescription spaceGridDescription;
    void* parent;
    
private:
    
    void iterateByIndexedCells(size_t from, size_t to, double dt, unsigned int currentThread)
    {
        IFractionCell** ppCells = allCells.data();
        for (size_t i=from; i!=to; i++)
        {
            ppCells[i]->calculateIntensiveQuantities();
            ppCells[i]->calculateDerivatives();
        }
        
        for (size_t i=from; i!=to; i++)
        {
            ppCells[i]->calculateFlowsEvolution(dt, currentThread);
            ppCells[i]->calculateSourceEvolution(dt, currentThread);
        }
            
    }
    
    uint m_threadsCount;
    
    //std::function<void(SpaceBase*, ThreadControlStructure*, double)> 
    ThreadsPool<double> threadsPool;
    //std::vector<AveragingBlock> averagingBlocks;
    std::vector<unsigned int> fractionsThatNeedAveraging;
    
    bool cellsAreIndexed;
    std::vector<IFractionCell*> allCells;
};


#endif // SPACE_BASE_H_INCLUDED
