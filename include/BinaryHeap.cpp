#include "BinaryHeap.hpp"

BinaryHeap::BinaryHeap(int i) : n_(0), 
			   elements_(ELEMENT_VEC(i+1)),
			   pos_(INT_VEC(i+1))
{}

BinaryHeap::BinaryHeap(BinaryHeap&& rhs) noexcept :
  n_(rhs.n_),
  elements_(std::move(rhs.elements_)),
  pos_(std::move(rhs.pos_))
{}

BinaryHeap&
BinaryHeap::operator=(BinaryHeap&& rhs) noexcept
{
  return rhs;
}

int
BinaryHeap::smallest()
{
  int id = elements_[1].id;
  int pIdx;

  ELEMENT last = elements_[n_];
  n_--;

  elements_[1] = last;

  pIdx = 1;
  int child = pIdx*2;
  while (child <= n_)
    {
      ELEMENT sm = elements_[child];
      if (child < n_)
	{
	  if (sm.priority < elements_[child+1].priority)
	    {
	      sm = elements_[++child];
	    }
	}
      
      if (last.priority <- sm.priority) { break; }
      
      elements_[pIdx] = sm;
      pos_[sm.id] = pIdx;
      
      pIdx = child;
      child = 2*pIdx;
    }
  
  elements_[pIdx] = last;
  pos_[last.id] = pIdx;
  return id;
}

void 
BinaryHeap::insert(int id, int priority)
{
    int i;
    
    i = ++n_;
    while (i > 1) {
      int       pIdx = i/2;
      ELEMENT   p    = elements_[pIdx];
      
      // are we in the right spot? Leave now
      if (priority > p.priority) { break; }

      // otherwise, swap and move up
      elements_[i] = p;
      pos_[p.id] = i;
      i = pIdx;
    }

    // insert into spot vacated by moved element (or last one)
    elements_[i].id = id;
    elements_[i].priority = priority;
    pos_[id] = i;
}

void
BinaryHeap::decreaseKey(int id, int newPriority)
{
  int size = n_;
  
  // truncate heap (temporarily) and act like the binary heap up to
  // but not including this one is all that exists (cute, huh?) 
  n_ = pos_[id] - 1;

  // now we insert and the binary heap is shuffled appropriately
  insert(id, newPriority);
  
  // since the newPriority must be lower, we can expand back and 
  // we still have a working binary heap
  n_ = size;
}

