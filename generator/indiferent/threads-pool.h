/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef THREADS_POOL_H_INCLUDED
#define THREADS_POOL_H_INCLUDED

#include <thread>
#include <atomic>
#include <semaphore.h>

template <class FunctionObject>
class ThreadsPool
{
public:
    ThreadsPool() :
        m_functionToRun(0),
        m_threads(0),
        m_runningThreads(0)
    {}
    
    ~ThreadsPool()
    {
        if (m_threads) delete[] m_threads;
        if (m_functionToRun) delete[] m_functionToRun;
    }
    
    void init(unsigned int threadsCount)
    {
        m_threadsCount = threadsCount;
        m_threads = new std::thread[m_threadsCount];
        m_functionToRun = new FunctionObject[m_threadsCount];
        threadCanStart = new sem_t[m_threadsCount];
        
        sem_init(&threadsDone, 0, 1);
        for (uint i=0; i<threadsCount; i++)
            sem_init(&(threadCanStart[i]), 0, 0);
    }
    
    void run()
    {
        m_runningThreads = m_threadsCount;
    }
    
private:
    static void threadFunction()
    {
        
    }
    
    int m_threadsCount;
    FunctionObject *m_functionToRun;
    std::thread *m_threads;
    std::atomic<int> m_runningThreads;
    sem_t threadsDone;
    sem_t *threadCanStart;
};

#endif
