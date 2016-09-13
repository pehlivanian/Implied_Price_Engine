#ifndef __FIXED_POOL_CONTROLLER_HPP__
#define __FIXED_POOl_CONTROLLER_HPP__

#include <cstdlib>
#include <algorithm>
#include <iostream>

using size_t = std::size_t;

struct fixed_pool_controller
{
  template<int N>
  fixed_pool_controller(char(&a)[N]);
  fixed_pool_controller(const fixed_pool_controller&)                   = delete;
  fixed_pool_controller& operator=(const fixed_pool_controller&) = delete;
  ~fixed_pool_controller()                                                            = default;

  void* allocate(size_t);
  size_t block_size() const;
  size_t capacity() const;
  void clear();
  bool empty() const;

private:
  void* pool_;
  size_t pool_size_;
  size_t block_size_;
};

template<int N>
inline fixed_pool_controller::fixed_pool_controller(char (&a)[N]) :
  pool_(a), pool_size_(N), block_size_(0) {}

void*
fixed_pool_controller::allocate(size_t size)
{
  if (!empty())
    return nullptr;
  
  block_size_ = std::max(size, sizeof(void*));
  size_t count = capacity();

  if (count == 0)
    return nullptr;  // Pool not big enough for 1 item

  return pool_;
}

size_t
fixed_pool_controller::block_size() const 
{
  return block_size_;
}

size_t
fixed_pool_controller::capacity() const
{
  return block_size_ ?  (pool_size_ / block_size_) : 0;
}

void
fixed_pool_controller::clear()
{
  block_size_ = 0;
}

bool
fixed_pool_controller::empty() const
{
  return block_size_ == 0;
}

#endif

