#ifndef __MEMPOOLTEST_H__
#define __MEMPOOLTEST_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <vector>
#include <algorithm>
// #include <python2.7/Python.h>

#include "report.hpp"
#include "problem.hpp"
#include "Graph.hpp"
#include "Graph_utils.hpp"
#include "BinaryHeap.hpp"
// #include "fixed_pool_controller.hpp"
// #include "pool_memory_manager.hpp"
#include "CMemPool.hpp"

// using mempool = pool_memory_manager<fixed_pool_controller>;
using mempool = CMemPool;
  
class mempooltester_stat
{
 public:
  mempooltester_stat(int p) : payload_(p) {}
  int get_payload() const { return payload_; }

  static void* operator new(size_t s)
    {
      return pool_.allocate(s);
    }

  static void operator delete(void* p)
  {
    pool_.deallocate(p);
  }

 private:
  int payload_;
  static mempool pool_;
};

mempool mempooltester_stat::pool_(500, 4);

class mempooltester_dyn
{
 public:
  mempooltester_dyn(int p) : payload_(p) {}
  int get_payload() const { return payload_; }

 private:
  int payload_;
};


#endif
