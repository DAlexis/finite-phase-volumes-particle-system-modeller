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
        poolContext.allThreadsShouldStop = false;
    }
    
    ~ThreadsPool()
    {
#ifdef USE_NATIVE_LOCKS
        sem_destroy(&(poolContext.threadsDone));
#endif
    }
    
    void wait()
    {
#ifdef USE_NATIVE_LOCKS
        sem_wait(&(poolContext.threadsDone));
#else
        //while (!poolContext.m_threadsDone) { }
        //while (!poolContext.checkAreAllFinished()) { }
        
        volatile int* isDone=poolContext.m_isThreadFinished.data();
        unsigned int size=threads.size();
        bool result = false;
        while(!result)
        {
            result = true;
            // Here we are cheching that all threads done
            for (unsigned int i=0; i<size; i++)
            {
                if (isDone[i] == 0) result = false;
            }
        }
#endif
    }
    
    void unlockThreads(FunctionArgType functionArg)
    {
        poolContext.m_runningThreadsCount = threads.size();
        poolContext.m_threadsDone = false;
        
#ifdef USE_NATIVE_LOCKS
        poolContext.m_runningThreadsCountAtomic = threads.size();
#else
        for (auto it = poolContext.m_isThreadFinished.begin(); it != poolContext.m_isThreadFinished.end(); it++)
        {
            *it = 0;
        }
#endif
        for (auto it = threads.begin(); it != threads.end(); it++)
        {
            it->currentFunctionArg = functionArg;
#ifdef USE_NATIVE_LOCKS
            sem_post(&(it->threadCanStart));
#else
            it->threadCanContinue = true;
#endif
        }
    }
    
    void stopThreads()
    {
        poolContext.allThreadsShouldStop = true;
        for (auto it = threads.begin(); it != threads.end(); it++)
        {
#ifdef USE_NATIVE_LOCKS
            sem_post(&(it->threadCanStart));
#else
            it->threadCanContinue = true;
#endif
        }
    }
    
    void addThread(FunctionObject&& function)
    {
        threads.push_back(ThreadWrapper(threads.size()));
#ifndef USE_NATIVE_LOCKS
        poolContext.m_isThreadFinished.push_back(0);
#endif
        threads.back().startThread(poolContext, function);
    }
    
private:

    class PoolContext
    {
    public:
        PoolContext() :
            m_runningThreadsCount(0),
            m_threadsDone(false)
        {
#ifdef USE_NATIVE_LOCKS
            sem_init(&threadsDone, 0, 0);
#endif
        }

#ifndef USE_NATIVE_LOCKS
        bool checkAreAllFinished()
        {
            for (auto it=m_isThreadFinished.begin(); it!=m_isThreadFinished.end(); it++)
            {
                if (*it != 1) return false;
            }
            return true;
        }
#endif
        
        uint m_runningThreadsCount;
#ifdef USE_NATIVE_LOCKS
        sem_t threadsDone;
        std::atomic<int> m_runningThreadsCountAtomic;
#else
        // Not bool because STL use bit's packing for vector of bool that cause a data race 
        std::vector<int> m_isThreadFinished;
#endif
        bool allThreadsShouldStop;
        volatile bool m_threadsDone;
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
            
#ifdef USE_NATIVE_LOCKS
            sem_init(&threadCanStart, 0, 0);
#endif
            m_pPoolContext = &poolContext;
            m_function = function;
            m_thread = new std::thread(&ThreadWrapper::threadFunction, this);
        }
        
        void threadFunction()
        {
            for (;;)
            {
#ifdef USE_NATIVE_LOCKS
                sem_wait(&threadCanStart);
#else
                while (!threadCanContinue) { }
                threadCanContinue = false;
#endif
                if (m_pPoolContext->allThreadsShouldStop) return;
                
                m_function(currentFunctionArg);
                
#ifdef USE_NATIVE_LOCKS
                // Checking if we are the last thread
                if (m_pPoolContext->m_runningThreadsCountAtomic.fetch_add(-1, std::memory_order_consume) == 1)
                    sem_post(&(m_pPoolContext->threadsDone));
#else
                m_pPoolContext->m_isThreadFinished[m_threadIndex] = 1;
#endif
                
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
                if (m_pPoolContext->allThreadsShouldStop) return;
                m_function(currentFunctionArg);
                
                // Checking if we are the last thread
                if (m_pPoolContext->m_runningThreadsCountAtomic.fetch_add(-1, std::memory_order_consume) == 1)
                    sem_post(&(m_pPoolContext->threadsDone));
                */
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
    };
    
    PoolContext poolContext;
    std::list<ThreadWrapper> threads;
    
};

#endif
