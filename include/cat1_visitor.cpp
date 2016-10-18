#include "cat1_visitor.hpp"

class NegativeCycle: public std::exception
{ const char* what() const throw() { return "Graph has a negative cycle"; }} NegativeCycle;

void
cat1_visitor::generate(DataElement* d)
{
  Graph* g = dynamic_cast<Graph*>(d);

  const int n = g->numVertices();
  pred_.assign(n, -1);
  dist_.assign(n, std::numeric_limits<int>::max());
    size_.assign(n, std::numeric_limits<int>::max());
  dist_[s_] = 0;

  // After n-1 times we ccan be guaranteed distances from s to all
  // vertices are properly computed to be shortest. So on the nth
  // pass, a change to any value guarantees there is a negative cycle.
  // Leave early if no changes are made.
  for(int i=1; i<=n; i++)
    {
      bool failOnUpdate = (i == n);
      bool leaveEarly = true;

      for(int u=0; u<n; u++)
	{
	  Graph::Graph_iterator ci = g->begin(u);
	  Graph::Graph_iterator last = g->end(u);
	  for(;
	      ci != last;
	      ++ci)
	    {
	      int v = ci->first;
	      long newLen = dist_[u];
	      newLen += (ci->second).first;
            int newSize = std::min(size_[u], (ci->second).second);
	      if (newLen < dist_[v])
		{
		  if (failOnUpdate) { throw NegativeCycle; }
		  dist_[v] = newLen;
            size_[v] = newSize;
		  pred_[v] = u;
		  leaveEarly = false;
		}
	    }
	}
      if (leaveEarly) { break; }
    }
}
