#ifndef __CAT1_VISITOR_HPP__
#define __CAT1_VISITOR_HPP__

#include <vector>
#include <algorithm>
#include <limits>
#include <exception>

#include "Visitor.hpp"
#include "DataElement.hpp"
#include "Graph.hpp"
#include "BinaryHeap.hpp"

class cat1_visitor : public Visitor
{
public:
  cat1_visitor() : s_(0) {}
  cat1_visitor(int s) : s_(s) {}
  std::vector<int> get_distance() const { return dist_; }
  std::vector<size_t> get_size() const { return size_; }
  std::vector<int> get_predecessor () const { return pred_; }
private:
  void generate(DataElement*) override;
  
  int s_;
  std::vector<int> pred_;
  std::vector<int> dist_;
  std::vector<size_t> size_;
};

#endif
