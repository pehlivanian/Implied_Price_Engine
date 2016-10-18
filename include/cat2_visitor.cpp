#include "cat2_visitor.hpp"

void 
cat2_visitor::generate(DataElement* d)
{
  Graph *g = dynamic_cast<Graph*>(d);

  const int n = g->numVertices();
  pred_.assign(n, -1);
  dist_.assign(n, std::numeric_limits<int>::max());
  size_.assign(n, std::numeric_limits<int>::max());
  dist_[s_] = 0;
  BinaryHeap pq(n);

  for(int u = 0; u < n; u++) { pq.insert(u, dist_[u]); }

  // Find vertex in ever shrinking set, V-S, whose dist_[] 
  // is smallest. Recompute potential new paths to update all shotest paths.
  while(!pq.isEmpty())
    {
      int u = pq.smallest(); // this also pops

      Graph::Graph_iterator ci = g->begin(u);
      Graph::Graph_iterator last = g->end(u);
      for(;
	  ci != last;
	  ++ci)
	{
	  int v = ci->first;
	  long newLen = dist_[u];
	  newLen += (ci->second).first;
	  size_t newSize = std::min(size_[u], (ci->second).second);
	  if (newLen < dist_[v])
	    {
	      pq.decreaseKey(v, newLen);
	      dist_[v] = newLen;
	      size_[v] = newSize;
	      pred_[v] = u;
	    }
	}
    }
}
