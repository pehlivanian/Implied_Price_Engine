#ifndef __MARKETGRAPH_HPP__
#define __MARKETGRAPH_HPP__

#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

#include "SecPair.hpp"
#include "Graph.hpp"

class MarketGraph : public Graph
{
public:

  using PropPair            = std::pair<int, SecPair>; 
  using VertexPropList   = std::vector<SecPair>;
  using EdgePropList     = std::list<PropPair>;

  using VertexPropIterator = VertexPropList::iterator;
  using EdgePropIterator   = EdgePropList::iterator;
  
  MarketGraph() : Graph(), 
		  vertex_props_(VertexPropList(1)),
		  edge_props_(std::vector<EdgePropList>(1)) {}
  MarketGraph(int n, bool d) : Graph(n, d),
			       vertex_props_(VertexPropList(n)),
			       edge_props_(std::vector<EdgePropList>(n)) {}
  MarketGraph(int n) : Graph(n),
		       vertex_props_(VertexPropList(n)),
		       edge_props_(std::vector<EdgePropList>(n)) {}

  MarketGraph(const MarketGraph&) = default;
  MarketGraph& operator=(const MarketGraph&) = default;
  MarketGraph(MarketGraph&&) noexcept;
  MarketGraph& operator=(MarketGraph&&) noexcept;

  SecPair edgeProp(int, int) const;
  SecPair vertexProp(int) const;
  void addEdgeProp(int, int, const SecPair&);
  void addVertexProp(int, const SecPair&);

  void update_distance(const std::vector<int>& d) { dist_ = d; }
  void update_predecessor(const std::vector<int>& p) { pred_ = p; }
  std::vector<int> get_distance() const { return dist_; }
  std::vector<int> get_predecessor() const { return pred_; }

  VertexPropIterator vertex_prop_begin() { return vertex_props_.begin(); }
  VertexPropIterator vertex_prop_end() { return vertex_props_.end(); }
  EdgePropIterator edge_prop_begin(int u) { return edge_props_[u].begin(); }
  EdgePropIterator edge_prop_end(int u) { return edge_props_[u].end(); }  

private:
  
  VertexPropList              vertex_props_;
  std::vector<EdgePropList>   edge_props_;
  std::vector<int> dist_;   // for shortest distance calculations
  std::vector<int> pred_;  // for shortest distance calculations
};

#endif
