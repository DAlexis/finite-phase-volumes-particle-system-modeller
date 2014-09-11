/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef THREADS_POOL_H_INCLUDED
#define THREADS_POOL_H_INCLUDED

#include <thread>
#include <atomic>
#include <semaphore.h>
#include <mutex>
#include <condition_variable>
#include <list>

#include <iostream>

template <class FunctionArgType>
class ThreadsPool
{
public:
    using FunctionObject = std::function<void(FunctionArgType)>;
    ThreadsPool()
    {
        sem_init(&(poolContext.threadsDone), 0, 0);
        poolContext.shouldStopThreads = false;
    }
    
    ~ThreadsPool()
    {
        sem_destroy(&(poolContext.threadsDone));
    }
    
    void wait()
    {
        /*
        sem_wait(&(poolContext.threadsDone));*/
        //while (!poolContext.m_threadsDone) { }
        //while (!poolContext.checkAreAllFinished()) { }
        volatile int* data=poolContext.m_threadFinished.data();
        unsigned int size=threads.size();
        bool result = false;
        while(!result)
        {
            result = true;
            for (unsigned int i=0; i<size; i++)
            {
                if (data[i] == 0) result = false;
            }
        }
    }
    
    void unlockThreads(FunctionArgType functionArg)
    {
        poolContext.m_runningThreadsCount = threads.size();
        poolContext.m_runningThreadsCountAtomic = threads.size();
        poolContext.m_threadsDone = false;
        for (auto it = poolContext.m_threadFinished.begin(); it != poolContext.m_threadFinished.end(); it++)
        {
            *it = 0;
        }
        
        for (auto it = threads.begin(); it != threads.end(); it++)
        {
            it->currentFunctionArg = functionArg;
            it->threadCanContinue = true;
            sem_post(&(it->threadCanStart));
        }
    }
    
    void stopThreads()
    {
        poolContext.shouldStopThreads = true;
        for (auto it = threads.begin(); it != threads.end(); it++)
        {
            it->threadCanContinue = true;
            sem_post(&(it->threadCanStart));
        }
    }
    
    void addThread(FunctionObject&& function)
    {
        threads.push_back(ThreadWrapper(threads.size()));
        poolContext.m_threadFinished.push_back(0);
        threads.back().startThread(poolContext, function);
    }
    
private:

    class PoolContext
    {
    public:
        PoolContext() :
            m_runningThreadsCount(0),
            m_threadsDone(false)
        {}
        
        bool checkAreAllFinished()
        {
            for (auto it=m_threadFinished.begin(); it!=m_threadFinished.end(); it++)
            {
                if (*it != 1) return false;
            }
            return true;
        }
        
        uint m_runningThreadsCount;
        std::mutex m_doneThreadsCounterMutex;
        sem_t threadsDone;
        std::atomic<int> m_runningThreadsCountAtomic;
        bool shouldStopThreads;
        volatile bool m_threadsDone;
        
        std::vector<int> m_threadFinished;
    };
    
    typedef std::atomic<int> DoneThreadsCounterType;
    class ThreadWrapper
    {
    public:
        ThreadWrapper(unsigned int index) :
            threadCanContinue(false),
            m_threadIndex(index),
            m_thread(0)
        { }
        
        ~ThreadWrapper()
        {
            if (m_thread) {
                sem_destroy(&threadCanStart);
                m_thread->join();
                delete m_thread;
            }
        }
        
        void startThread(PoolContext& poolContext, FunctionObject &function)
        {
            m_pPoolContext = &poolContext;
            sem_init(&threadCanStart, 0, 0);
            m_function = function;
            m_thread = new std::thread(&ThreadWrapper::threadFunction, this);
        }
        
        void threadFunction()
        {
            for (;;)
            {
                while (!threadCanContinue) { }
                threadCanContinue = false;
                if (m_pPoolContext->shouldStopThreads) return;
                
                m_function(currentFunctionArg);
                
                
                m_pPoolContext->m_threadFinished[m_threadIndex] = 1;
                //std::cout << "?" << m_threadIndex << std::flush;
                /*
                if (m_pPoolContext->checkAreAllFinished())
                {
                    //std::cout << "!" << m_threadIndex << std::flush;
                    m_pPoolContext->m_threadsDone = true;
                }*/
                
                /*
                if (m_pPoolContext->m_runningThreadsCountAtomic.fetch_add(-1, std::memory_order_consume) == 1) {
                    m_pPoolContext->m_threadsDone = true;
                    //sem_post(&(m_pPoolContext->threadsDone));
                }*/
                
                /*
                sem_wait(&threadCanStart);
                if (m_pPoolContext->shouldStopThreads) return;
                m_function(currentFunctionArg);
                
                // Checking if we are the last thread
                
                if (m_pPoolContext->m_runningThreadsCountAtomic.fetch_add(-1, std::memory_order_consume) == 1)
                    sem_post(&(m_pPoolContext->threadsDone));
                */
                    
                /*
                m_doneThreadsCounterMutex->lock();
                (*m_pCounter)--;
                if (*m_pCounter == 0)
                    sem_post(m_pThreadsDone);
                
                m_doneThreadsCounterMutex->unlock();*/
            }
        }
        
        sem_t threadCanStart;
        FunctionArgType currentFunctionArg;
        
        volatile bool threadCanContinue;
        
    private:
        int m_threadIndex;
        PoolContext *m_pPoolContext;
        std::thread *m_thread;
        FunctionObject m_function;
        uint *m_pCounter;
        
        bool *m_pThreadsAreLocked;
    };
    
    PoolContext poolContext;
    std::list<ThreadWrapper> threads;
    
};

#endif
