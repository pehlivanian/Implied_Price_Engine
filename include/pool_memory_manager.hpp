#ifndef __POOL_MEMORY_MANAGER_HPP__
#define __POOL_MEMORY_MANAGER_HPP__

#include <cstdlib>
#include <memory>
#include <iostream>
#include <exception>

using size_t = std::size_t;

template<typename Pool>
struct pool_memory_manager
{
  template<int N>
  pool_memory_manager(char (&a)[N]);
  ~pool_memory_manager()                                                                = default;
  pool_memory_manager(const pool_memory_manager&)                   = delete;
  pool_memory_manager& operator=(const pool_memory_manager&) = delete;
  
  void* allocate(size_t);
  void deallocate(void*);
  size_t block_size() const;
  inline size_t capacity() const;
  inline void clear();
  inline bool empty() const;

private:
  struct free_block
  {
    free_block* next;
  };

  free_block* free_ptr_;
  size_t block_size_;
  Pool pool_;

};

#include "pool_memory_manager_impl.hpp"

#endif
