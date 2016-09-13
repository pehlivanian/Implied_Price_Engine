#ifndef __BINARYHEAP_HPP__
#define __BINARYHEAP_HPP__

#include <memory>
#include <vector>
#include <algorithm>

typedef struct elt {
  int     id;            
  int     priority;
} ELEMENT;

typedef std::vector<ELEMENT> ELEMENT_VEC;
typedef std::vector<int> INT_VEC;

class BinaryHeap
{
public:
  BinaryHeap() = default;

  BinaryHeap(int);
  BinaryHeap(const BinaryHeap&) = default;
  BinaryHeap& operator=(const BinaryHeap&) = default;
  BinaryHeap(BinaryHeap&&) noexcept;
  BinaryHeap& operator=(BinaryHeap&&) noexcept;

  bool isEmpty() { return (n_ == 0); }
  int smallest();
  void insert(int, int);
  // The reason we create our own binary heap
  // instead of using std::priority_queue<T> or
  // a boost object : efficient implementation of the 
  // following
  void decreaseKey(int, int);

private:
  int n_;
  ELEMENT_VEC elements_;
  INT_VEC pos_;

  // Performance metrics triggered in makefile
  long _numComparisons;
  long _numInsert;
  long _numSwaps;
  long _numSmallest;
  long _numDecrease;
};

#endif
