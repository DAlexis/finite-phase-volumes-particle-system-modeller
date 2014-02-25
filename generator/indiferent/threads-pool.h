/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef THREADS_POOL_H_INCLUDED
#define THREADS_POOL_H_INCLUDED

#include <thread>
#include <atomic>
#include <semaphore.h>

#include <list>
#include <iostream>

template <class FunctionObject, class FunctionArgType>
class ThreadsPool
{
public:
    ThreadsPool() :
        m_runningThreadsCount(0)
    {
        sem_init(&threadsDone, 0, 1);
        shouldStopThreads = false;
    }
    
    ~ThreadsPool()
    {
        sem_destroy(&threadsDone);
    }
    
    void wait()
    {
        //std::cout << "== Waiting threads to be done..." << std::endl;
        sem_wait(&threadsDone);
        sem_post(&threadsDone);
        //std::cout << "== Yes, done..." << std::endl;
    }
    
    void unlockThreads(FunctionArgType functionArg)
    {
        sem_wait(&threadsDone);
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
        sem_wait(&threadsDone);
        for (auto it = threads.begin(); it != threads.end(); it++)
            sem_post(&(it->threadCanStart));
    }
    
    void addThread(FunctionObject& function)
    {
        threads.push_back(ThreadData());
        threads.back().startThread(m_runningThreadsCount, threadsDone, function, shouldStopThreads);
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
        
        void startThread(DoneThreadsCounterType& counter, sem_t& threadsDone, FunctionObject &function, bool& shouldStop)
        {
            m_pCounter = &counter;
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
                //std::cout << "I'm waiting for signal to start job..." << std::endl;
                sem_wait(&threadCanStart);
                //std::cout << "Hm. ";
                if (*m_pShouldStop) return;
                //std::cout << "Yes, I'm going to do job." << std::endl;
                m_function(currentFunctionArg);
                (*m_pCounter)--;
                if (*m_pCounter == 0)
                    sem_post(m_pThreadsDone);
            }
        }
        
        sem_t threadCanStart;
        FunctionArgType currentFunctionArg;
        
    private:
        std::thread *m_thread;
        FunctionObject m_function;
        DoneThreadsCounterType *m_pCounter;
        sem_t *m_pThreadsDone;
        bool *m_pShouldStop;
        
    };
    
    std::atomic<int> m_runningThreadsCount;
    sem_t threadsDone;
    std::list<ThreadData> threads;
    bool shouldStopThreads;
};

#endif
