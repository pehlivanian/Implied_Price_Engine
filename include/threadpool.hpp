#ifndef __THREADPOOL_HPP__
#define __THREADPOOL_HPP__

#include <iostream>
#include <memory>
#include <future>
#include <thread>
#include <mutex>
#include <vector>
#include <type_traits>

#include "threadsafe_queue.h"

class function_wrapper;
static void swap(function_wrapper &first,
		 function_wrapper &second);

class function_wrapper
{
 public:
   template<typename F>
     function_wrapper(F&& f) :
     impl(new impl_type<F>(std::move(f))) {}

   void operator()() { impl->call(); }
   
   function_wrapper() = default;

   function_wrapper& operator=(function_wrapper&& other)
     {
	impl = std::move(other.impl);
	return *this;
     }
   
   // No-throw swap
   // Destructor
   // Move constructor
   // Copy constructor
   // Copy assignment operator

   friend void swap(function_wrapper &first,
		    function_wrapper &second);
   ~function_wrapper() = default;
   function_wrapper(function_wrapper&& other) :
     impl(std::move(other.impl)) {}
   function_wrapper(const function_wrapper& other) :
     impl(other.impl)
     {}   
   function_wrapper(function_wrapper& other) :
     impl(other.impl) {}
   function_wrapper& operator=(function_wrapper other)
     {
       function_wrapper tmp(other);
       swap(*this, other);
       return *this;
     }

 private:
   struct impl_base
     {
	virtual void call() = 0;
	virtual ~impl_base() {}
     };

   template<typename F>
     struct impl_type : impl_base
     {
	F f;
	impl_type(F&& f_) : f(std::move(f_)) {}
	void call() { f(); }
     };   

   std::shared_ptr<impl_base> impl;

};

static void swap(function_wrapper &first,
	  function_wrapper &second)
{
   using std::swap;
   swap(first.impl, second.impl);
}


class join_threads
{
   std::vector<std::thread>& threads_;
 public:
   explicit join_threads(std::vector<std::thread>& threads) :
     threads_(threads) {}
   ~join_threads()
     {
	for_each(threads_.begin(),threads_.end(),
		 [](std::thread& t){if (t.joinable()) { t.join(); }});
     }
};

class threadpool
{
   std::atomic_bool done;
   threadsafe_queue<function_wrapper> work_queue;
   std::vector<std::thread> threads;
   join_threads joiner;

   void worker_thread()
     {
	while(!done)
	  {
	     function_wrapper task;
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
   threadpool() :
     done(false), 
     joiner(threads)
       {
	 unsigned const thread_count = std::thread::hardware_concurrency();
	 
	 try
	   {
	     for(unsigned i=0;i<thread_count;++i)
	       {
		 threads.push_back(std::thread(
					       &threadpool::worker_thread,this));
	       }
	   }
	 catch(...)
	   {
	     done = true;
	     throw;
	   }
       }
  
   ~threadpool()
     {
	done = true;
     }

  template<typename FunctionType>
  std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f)
  {
    using result_type = typename std::result_of<FunctionType()>::type;
    std::packaged_task<result_type()> task(std::move(f));
    std::future<result_type> res(task.get_future());
    work_queue.push(std::move(task));
    return res;
  }
  
};

#endif
