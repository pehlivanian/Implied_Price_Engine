#ifndef __POOL_MEMORY_MANAGER_IMPL_HPP__
#define __POOL_MEMORY_MANAGER_IMPL_HPP__

template<typename Pool>
template<int N>
inline pool_memory_manager<Pool>::pool_memory_manager(char(&a)[N]) :
  free_ptr_(nullptr), block_size_(0), pool_(a) {}

template<typename Pool>
inline void* 
pool_memory_manager<Pool>::allocate(size_t size)
{
  std::cerr << "DEBUG: empty(): " << empty() << " : " << free_ptr_  << " : " << capacity() << " : " << size << "\n";
  if (empty())
    {
      free_ptr_ = reinterpret_cast<free_block*>(pool_.allocate(size));
#if 1
      for(int i=0, cap = capacity()-1; i<cap; ++i)
	{
	  std::cerr << "DEBUG: free_ptr_: " << i << " : " << free_ptr_ << " : " << cap << "\n";
	  free_ptr_->next = reinterpret_cast<free_block*>(free_ptr_ + size);
	  free_ptr_ = free_ptr_->next;
	}
#endif
      block_size_ = size;
      if (empty())
	throw std::bad_alloc();
    }
  if (size != block_size_)
    throw std::bad_alloc();

  auto p = free_ptr_;
  free_ptr_ = free_ptr_->next;
  return p;
}

template<typename Pool>
inline void
pool_memory_manager<Pool>::deallocate(void* p)
{
  if (p == nullptr)
    return;

  auto fp = reinterpret_cast<free_block*>(p);
  fp->next = free_ptr_;
  free_ptr_ = fp;
}

template<typename Pool>
inline size_t
pool_memory_manager<Pool>::block_size() const
{
  return block_size_;
}

template<typename Pool>
inline size_t 
pool_memory_manager<Pool>::capacity() const
{
  return pool_.capacity();
}

template<typename Pool>
inline void
pool_memory_manager<Pool>::clear()
{
  free_ptr_ = nullptr;
  pool_.clear();
}

template<typename Pool>
inline bool
pool_memory_manager<Pool>::empty() const
{
  return pool_.empty();
}

#endif
