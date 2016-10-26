//
// Created by charles on 10/25/16.
//

#ifndef __LW_THREADPOOL_HPP__
#define __LW_THREADPOOL_HPP__


#include <iostream>
#include <memory>
#include <thread>
#include <future>
#include <mutex>
#include <vector>
#include <type_traits>

#include "threadsafe_queue.h"


class LW_join_threads
{
    std::vector<std::thread>& threads_;
public:
    explicit LW_join_threads(std::vector<std::thread>& threads) :
            threads_(threads) {}
    ~LW_join_threads()
    {
        for_each(threads_.begin(),threads_.end(),
                 [](std::thread& t){if (t.joinable()) { t.join(); }});
    }
};


template<typename R>
class LW_threadpool {
private:

    using fn_type = std::function<R()>;

    std::atomic_bool done;
    threadsafe_queue<fn_type> work_queue;
    std::vector<std::thread> threads;
    LW_join_threads joiner;

    void worker()
    {
        while(!done)
        {
            fn_type task;
            if (work_queue.try_pop(task))
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }
public:
    LW_threadpool() : done(false), joiner(threads)
    {
        // unsigned const thread_count = std::thread::hardware_concurrency();
        unsigned const thread_count = 1;

        try
        {
            for(unsigned i=0;i<thread_count;++i)
            {
                threads.push_back(std::thread(
                        &LW_threadpool::worker, this));
            }
        }
        catch(...)
        {
            done = true;
            throw;
        }

    }
    ~LW_threadpool() { done = true; }

    inline void submit(const fn_type& f) { work_queue.push(f); }

};

#endif
