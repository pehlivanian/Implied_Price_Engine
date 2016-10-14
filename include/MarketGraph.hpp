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
		  edge_props_(std::vector<EdgePropList>(1)),
		  dist_(2),
                  size_(2),
		  pred_(2) {}
  MarketGraph(int n, bool d) : Graph(n, d),
			       vertex_props_(VertexPropList(n)),
			       edge_props_(std::vector<EdgePropList>(n)),
			       dist_(2),
                               size_(2),
			       pred_(2) {}
  MarketGraph(int n) : Graph(n),
		       vertex_props_(VertexPropList(n)),
		       edge_props_(std::vector<EdgePropList>(n)),
		       dist_(2),
                       size_(2),
		       pred_(2) {}
  
  MarketGraph(const MarketGraph&) = default;
  MarketGraph& operator=(const MarketGraph&) = default;
  MarketGraph(MarketGraph&&) noexcept;
  MarketGraph& operator=(MarketGraph&&) noexcept;

  SecPair edgeProp(int, int) const;
  SecPair vertexProp(int) const;
  void addEdgeProp(int, int, const SecPair&);
  void addVertexProp(int, const SecPair&);

  void update_distance(const std::vector<int>& d, int b) { dist_[b] = d; }
	void update_size(const std::vector<size_t>& s, int b) { size_[b] = s; }
  void update_predecessor(const std::vector<int>& p, int b) { pred_[b] = p; }
  std::vector<int> get_distance(int b) const { return dist_[b]; }
	std::vector<size_t> get_size(int b) const { return size_[b]; }
  std::vector<int> get_predecessor(int b) const { return pred_[b]; }

  VertexPropIterator vertex_prop_begin() { return vertex_props_.begin(); }
  VertexPropIterator vertex_prop_end() { return vertex_props_.end(); }
  EdgePropIterator edge_prop_begin(int u) { return edge_props_[u].begin(); }
  EdgePropIterator edge_prop_end(int u) { return edge_props_[u].end(); }  

private:
  
  VertexPropList              vertex_props_;
  std::vector<EdgePropList>   edge_props_;
  std::vector<std::vector<int>> dist_;       // for bid, ask shortest distance calculations
    std::vector<std::vector<size_t>> size_;  // for bid, ask size calculations
  std::vector<std::vector<int>> pred_;       // for bid, ask shortest distance calculations
};

#endif
