#ifndef __CAT2_VISITOR_HPP__
#define __CAT2_VISITOR_HPP__

#include <vector>
#include <algorithm>
#include <limits>

#include "Visitor.hpp"
#include "DataElement.hpp"
#include "Graph.hpp"
#include "BinaryHeap.hpp"

class cat2_visitor : public Visitor
{
public:
  cat2_visitor() : s_(0) {}
  cat2_visitor(int s) : s_(s) {}
  std::vector<int> get_distance() const { return dist_; }
  std::vector<int> get_predecessor() const { return pred_; }
private:
  void generate(DataElement*) override;

  int s_;
  std::vector<int> pred_;
  std::vector<int> dist_;
};

#endif
