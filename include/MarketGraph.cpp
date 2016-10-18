#include "MarketGraph.hpp"

MarketGraph::MarketGraph(MarketGraph&& rhs) noexcept :
   vertex_props_(std::move(rhs.vertex_props_)),
   edge_props_(std::move(rhs.edge_props_)) {}

MarketGraph&
MarketGraph::operator=(MarketGraph&& rhs) noexcept
{
  return rhs;
}

SecPair
MarketGraph::edgeProp(int u, int v) const
{

  auto ib = edge_props_[u].begin();
  auto ie = edge_props_[u].end();
  while(ib != ie)
    {
      if (ib->first == v)
	return ib->second;
      ++ib;
    }
  return SecPair(-1, -1, 1);
}

SecPair
MarketGraph::vertexProp(int u) const
{
  return vertex_props_[u];
}

void
MarketGraph::addEdgeProp(int u, int v, const SecPair& p)
{
  edge_props_[u].push_front(std::make_pair(v, p));
}

void
MarketGraph::addVertexProp(int u, const SecPair& p)
{
  vertex_props_[u] = p;
}
