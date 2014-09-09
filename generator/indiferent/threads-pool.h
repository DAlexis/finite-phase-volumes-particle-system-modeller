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

template <class FunctionArgType>
class ThreadsPool
{
public:
    using FunctionObject = std::function<void(FunctionArgType)>;
    ThreadsPool() :
        m_runningThreadsCount(0)
    {
        sem_init(&threadsDone, 0, 0);
        shouldStopThreads = false;
    }
    
    ~ThreadsPool()
    {
        sem_destroy(&threadsDone);
    }
    
    void wait()
    {
        sem_wait(&threadsDone);
    }
    
    void unlockThreads(FunctionArgType functionArg)
    {
        m_runningThreadsCount = threads.size();
        for (auto it = threads.begin(); it != threads.end(); it++)
        {
            it->currentFunctionArg = functionArg;
            sem_post(&(it->threadCanStart));
        }
    }
    
    void stopThreads()
    {
        shouldStopThreads = true;
        for (auto it = threads.begin(); it != threads.end(); it++)
            sem_post(&(it->threadCanStart));
    }
    
    void addThread(FunctionObject&& function)
    {
        threads.push_back(ThreadData());
        threads.back().startThread(threadsDone, function, shouldStopThreads, m_doneThreadsCounterMutex, m_runningThreadsCount);
    }
    
private:
    typedef std::atomic<int> DoneThreadsCounterType;
    class ThreadData
    {
    public:
        ThreadData() :
            m_thread(0)
        { }
        
        ~ThreadData()
        {
            if (m_thread) {
                sem_destroy(&threadCanStart);
                m_thread->join();
                delete m_thread;
            }
        }
        
        void startThread(sem_t& threadsDone, FunctionObject &function, bool& shouldStop, std::mutex& doneThreadsCounterMutex, uint& doneThreadsCounter)
        {
            m_doneThreadsCounterMutex = &doneThreadsCounterMutex;
            m_pCounter = &doneThreadsCounter;
            m_pThreadsDone = &threadsDone;
            m_pShouldStop = &shouldStop;
            sem_init(&threadCanStart, 0, 0);
            m_function = function;
            m_thread = new std::thread(&ThreadData::threadFunction, this);
        }
        
        void threadFunction()
        {
            for (;;)
            {
                sem_wait(&threadCanStart);
                if (*m_pShouldStop) return;
                m_function(currentFunctionArg);
                
                m_doneThreadsCounterMutex->lock();
                (*m_pCounter)--;
                if (*m_pCounter == 0)
                    sem_post(m_pThreadsDone);
                m_doneThreadsCounterMutex->unlock();
            }
        }
        
        sem_t threadCanStart;
        FunctionArgType currentFunctionArg;
        
    private:
        std::thread *m_thread;
        FunctionObject m_function;
        uint *m_pCounter;
        sem_t *m_pThreadsDone;
        bool *m_pShouldStop;
        std::mutex *m_doneThreadsCounterMutex;
    };
    
    uint m_runningThreadsCount;
    std::mutex m_doneThreadsCounterMutex;
    sem_t threadsDone;
    std::list<ThreadData> threads;
    bool shouldStopThreads;
};

#endif
