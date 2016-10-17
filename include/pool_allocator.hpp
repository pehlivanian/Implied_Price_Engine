#ifndef __POOL_ALLOCATOR_HPP__
#define __POOL_ALLOCATOR_HPP__

#include <cstddef>
#include <cstdio>

template <std::size_t N>
class arena
{
  static const std::size_t alignment = 16;
  alignas(alignment) char buf_[N];
  char* ptr_;

  std::size_t 
  align_up(std::size_t n) {return n + ((alignment-1) & ~(alignment-1));}

public:
  arena() : ptr_(buf_) {}
  arena(const arena&) = delete;
  arena& operator=(const arena&) = delete;

  char* allocate(std::size_t n)
  {
    n = align_up(n);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
    if (buf_ + N - ptr_ >= n)
      {
	char* r = ptr_;
	ptr_ += n;
	return r;
      }
#pragma GCC diagnostic pop
    return static_cast<char*>(::operator new(n));
  }
  void deallocate(char* p, std::size_t n)
  {
    n = align_up(n);
    if (buf_ <= p && p < buf_ + N)
      {
	if (p + n == ptr_)
	  ptr_ = p;
      }
    else
      ::operator delete(p);
  }
};

template <class T, std::size_t N>
class stack_allocator
{
  arena<N>& a_;
public:
  typedef T value_type;

public:
  template <class U> struct rebind {typedef stack_allocator<U, N> other;};
  
  explicit stack_allocator(arena<N>& a) : a_(a) {}
  // stack_allocator(arena<N>&& a) : a_(std::move(a)) {}
  template <class U>
  stack_allocator(const stack_allocator<U, N>& a)
    : a_(a.a_) {}
  stack_allocator(stack_allocator&&)                         = default;
  stack_allocator(const stack_allocator&)                   = default;
  stack_allocator& operator=(const stack_allocator&) = delete;
  
  T* allocate(std::size_t n)
  {
    return reinterpret_cast<T*>(a_.allocate(n*sizeof(T)));
  }
  void deallocate(T* p, std::size_t n)
  {
    // XXX
    // alternatively, nop
    ;
    // a_.deallocate(reinterpret_cast<char*>(p), n*sizeof(T));
  }
  
  template <class T1, std::size_t N1, class U, std::size_t M>
  friend
  bool
  operator==(const stack_allocator<T1, N1>& x, const stack_allocator<U, M>& y);

  template <class U, std::size_t M> friend class stack_allocator;
};

template <class T, std::size_t N, class U, std::size_t M>
bool
operator==(const stack_allocator<T, N>& x, const stack_allocator<U, M>& y)
{
  return (N == M) && (&x.a_ == &y.a_);
}

template <class T, std::size_t N, class U, std::size_t M>
bool
operator!=(const stack_allocator<T, N>& x, const stack_allocator<U, M>& y)
{
  return !(x == y);
}

#endif
