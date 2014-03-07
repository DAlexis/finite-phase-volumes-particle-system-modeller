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

template<int SpaceDimension, class GridElementType>
class SpaceBase : public Grid<SpaceDimension, GridElementType>, public ISpace
{
public:
    typedef Grid<SpaceDimension, GridElementType> SpaceGridInstance;
    typedef GridElementBase<SpaceDimension> SpaceGridElementInstance;
    
    SpaceBase() :
        m_threadsCount(1),
        threadsControls(0),
        borderMutexes(0)
        {}
    
    virtual ~SpaceBase()
    {
        if (threadsControls) delete[] threadsControls;
        if (borderMutexes) delete[] borderMutexes;
    }
    
    void initQuantities()
    {
        for (size_t i=0; i<this->elementsCount; i++)
            this->elements[i].initQuantities();
    }
    
    void calculateEvolution(double dt)
    {
        if (m_threadsCount == 1) {
            for (size_t i=0; i<this->elementsCount; i++)
                this->elements[i].calculateEvolution(dt);
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
    
    void initThreads(uint threadsCount)
    {
        m_threadsCount = threadsCount;
        if (m_threadsCount == 1)
            return;
        
        threadsControls = new ThreadControlStructure[m_threadsCount];
        // First space axis is devided
        // @todo Optimal is when 'longest' axis is devided to threads.
        uint borderBeginCoords_ui[SpaceDimension];
        for (uint i=0; i<SpaceDimension; i++)
            borderBeginCoords_ui[i] = 0;
        
        uint borderEndCoords_ui[SpaceDimension];
        for (uint i=0; i<SpaceDimension; i++)
            borderEndCoords_ui[i] = this->gridDescription->axis[i].getSegmentsCount()-1;
        
        borderMutexes = new std::mutex[m_threadsCount-1];
        
        threadsControls[0].begin = 0;
        threadsControls[m_threadsCount-1].end = this->elementsCount-1;
        
        for (uint i=0; i<m_threadsCount-1; i++) {
            threadsControls[i].rightBorderMutex = &(borderMutexes[i]);
            threadsControls[i+1].leftBorderMutex = &(borderMutexes[i]);
            
            uint borderAxisIndex = this->gridDescription->axis[0].getSegmentsCount() / m_threadsCount * (i+1);
            
            borderBeginCoords_ui[0] = borderAxisIndex;
            borderEndCoords_ui[0]   = borderAxisIndex;
            threadsControls[i].rightBorderBegin = this->getElementIndex_ui(borderBeginCoords_ui);
            threadsControls[i].end              = this->getElementIndex_ui(borderEndCoords_ui);
            
            borderBeginCoords_ui[0] = borderAxisIndex+1;
            borderEndCoords_ui[0]   = borderAxisIndex+1;
            threadsControls[i+1].begin         = this->getElementIndex_ui(borderBeginCoords_ui);
            threadsControls[i+1].leftBorderEnd = this->getElementIndex_ui(borderEndCoords_ui);
            
            std::function<void(double)> functionForThisBlock = std::bind(miltithreadedCalculateEvolution, this, &(threadsControls[i]), std::placeholders::_1);
            threadsPool.addThread(functionForThisBlock);
        }
        
        std::function<void(double)> functionForLastBlock = std::bind(miltithreadedCalculateEvolution, this, &(threadsControls[m_threadsCount-1]), std::placeholders::_1);
        threadsPool.addThread(functionForLastBlock);
    }
    
    void stopThreads()
    {
        threadsPool.stopThreads();
    }
    
    // Realisation of ISapce interface
    IFractionsPool* getCell_d(const double* coords) { return this->accessElement_d(coords); }
    IFractionsPool* getCell_ui(const uint* coords) { return this->accessElement_ui(coords); }
    const Axis* getAxisDescription(unsigned int axis) { return &(this->gridDescription->axis[axis]); }
    
    typename SpaceGridInstance::GridDescription spaceGridDescription;
    void* parent;
    
private:
    struct ThreadControlStructure
    {
        ThreadControlStructure() :
            begin(0),
            end(0),
            leftBorderEnd(-1),
            rightBorderBegin(-1)
            {}
        int begin;
        int end;
        int leftBorderEnd;
        int rightBorderBegin;
        
        std::mutex* leftBorderMutex;
        std::mutex* rightBorderMutex;
    };
    
    static void miltithreadedCalculateEvolution(SpaceBase* object, ThreadControlStructure *threadControl, double dt)
    {
        int i = threadControl->begin;
        //std::cout << "begin: " << threadControl->begin << " end: " << threadControl->end << std::endl;
        if (threadControl->leftBorderEnd != -1) // If we have left border
        {
            threadControl->leftBorderMutex->lock();
            for (; i <= threadControl->leftBorderEnd; i++)
                object->elements[i].calculateEvolution(dt);
            threadControl->leftBorderMutex->unlock();
        }
        
        if (threadControl->rightBorderBegin != -1) // If we have right border
        {
            // Middle, no border
            for (; i< threadControl->rightBorderBegin; i++)
                object->elements[i].calculateEvolution(dt);
            
            // Right border
            threadControl->rightBorderMutex->lock();
            for (; i<=threadControl->end; i++)
                object->elements[i].calculateEvolution(dt);
            threadControl->rightBorderMutex->unlock();
        } else {
            // No right border, simply iterating to end
            for (; i<=threadControl->end; i++)
                object->elements[i].calculateEvolution(dt);
        }
        
        if (threadControl->rightBorderBegin != -1)
        {
            threadControl->rightBorderMutex->lock();
            for (; i<=threadControl->end; i++)
                object->elements[i].calculateEvolution(dt);
            threadControl->rightBorderMutex->unlock();
        }
    }
    
    uint m_threadsCount;
    ThreadControlStructure *threadsControls;
    std::mutex* borderMutexes;
    //std::function<void(SpaceBase*, ThreadControlStructure*, double)> 
    ThreadsPool<std::function<void(double)>, double> threadsPool;
};


#endif // SPACE_BASE_H_INCLUDED
