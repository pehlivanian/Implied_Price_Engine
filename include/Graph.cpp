#include "Graph.hpp"

Graph::Graph(const Graph& rhs)
{
  SERIALIZE_WRITES;
  n_ = rhs.n_;
  directed_ = rhs.directed_;
  std::copy(rhs.vertices_.begin(), rhs.vertices_.end(), vertices_.begin());
}

Graph::Graph(Graph&& rhs) noexcept :
  n_(rhs.n_),
  directed_(rhs.directed_),
  vertices_(std::move(rhs.vertices_))
{}

Graph&
Graph::operator=(Graph&& rhs) noexcept
{
  return rhs;
}	

inline bool
Graph::isEdge(int v1, int v2) const
{
  SERIALIZE_READS;
  for(CVertexIterator ei = vertices_[v1].begin(),
	ee = vertices_[v1].end();
      ei != ee;
      ++ei)
    {
      if (ei->first == v2)
	return true;
    }
  return false;
}

inline bool
Graph::isEdge(int v1, int v2, std::pair<int,size_t>& w) const
{
  SERIALIZE_READS;
  for(CVertexIterator ei = vertices_[v1].begin(),
	ee = vertices_[v1].end();
      ei != ee;
      ++ei)
    {
      if (ei->first == v2)
	{
	  w = ei->second;
	  return true;
	}
    }
  return false;
}

inline int
Graph::edgeWeight(int v1, int v2) const
{
  SERIALIZE_READS;
    CVertexIterator ei = vertices_[v1].begin();
    for(CVertexIterator ee = vertices_[v1].end();
      ei != ee;
      ++ei)
    {
      if (ei->first == v2)
	    break;
    }
    return (ei->second).first;

}

void 
Graph::load (std::string filename) {
  FILE *fp = fopen (filename.c_str(), "r");
  int nv, ne;

  // sufficient space
  char buf[4096];

  char* i_ = fgets (buf, 4096, fp);

  int nr = fscanf (fp, "%d %d %s\n", &nv, &ne, buf);
  if (nr != 3) {
    std::cout << "Invalid file format " << filename << "\n";
    n_=0;
    return;
  }

  if (!strcmp (buf, "directed")) {
    directed_ = true;
  } else {
    directed_ = false;
  }

  // vertices_.resize(nv);
  for(int i=0; i<nv; ++i)
    {
      arena<POOL_SIZE> ar;
      vertices_.emplace_back(A<IPair, POOL_SIZE>(ar));
    }
  n_ = nv;

  while (ne-- > 0) {
    int src, tgt, weight1;
      size_t weight2;

    int nr = fscanf (fp, "%d,%d,%d,%lu\n", &src, &tgt, &weight1, &weight2);
    if (nr == 2) {
      addEdge (src, tgt);
    } else if (nr == 4) {
      addEdge (src, tgt, std::make_pair(weight1, weight2));
    }
  }

  fclose(fp);
}

void
Graph::addEdge(int v1, int v2, const std::pair<int, size_t>& p)
{
    // No need to SERIALIZE as we are assuming this is only called on init_()
    vertices_[v1].emplace_front(v2, p);
    // undirected have both
    if (!directed_)
        vertices_[v2].emplace_front(v1, p);
}

void
Graph::addEdge(int v1, int v2, int w1, int w2)
{

    addEdge(v1, v2, std::make_pair(w1, w2));
}

void
Graph::addEdge(int v1, int v2)
{
  // No need to SERIALLIZE as we are assuming this is only called on init_()
  addEdge(v1, v2, 1, 0);
}


bool
Graph::removeEdge(int v1, int v2)
{
  // No need to SERIALIZE as we are assuming this is only called on init_()
  bool found = false;
  for( VertexIterator ei = vertices_[v1].begin(),
	 ee = vertices_[v2].end();
       ei != ee;
       ++ei)
    {
      if (ei->first == v2)
	{
	  vertices_[v1].remove(*ei);
	  found = true;
	  break;
	}
    }
  if (!found) return false;

  if (!directed_) {
    for (VertexList::const_iterator ei = vertices_[v2].begin();
	 ei != vertices_[v2].end(); 
	 ++ei) {
      if (ei->first == v1) {
	vertices_[v2].remove(*ei);
	break;
      }
    }
  }
  return true;
}

// This function will assuredly be hot
bool
Graph::updateEdgeWeight(int v1, int v2, int w1, size_t w2)
{
  SERIALIZE_WRITES;
  CVertexIterator eb = vertices_[v1].before_begin();
  for( CVertexIterator ei = vertices_[v1].begin(),
	 ee = vertices_[v1].end();
       ei != ee;
       ++ei)
    {
      if ((ei->first) == v2)
	{
#if 0
        // XXX
        // Deal with undirected graphs later
        // TIE FOR SECOND
          (ei->second) = std::make_pair(w1, w2);
          // NEXT SLOWEST
          // (ei->second).first = w1;
          // (ei->second).second = w2;
          // SLOWEST
        // std::swap((ei->second).first,  w1);
        // std::swap((ei->second).second, w2);
        return true;
#endif
          // FASTEST, EVEN SO
#if 1
	  // Remove edge
          // Although technically we don't need to if
          // we always emplace_front
	  ei = vertices_[v1].erase_after(eb);
	  // Add new edge
	  vertices_[v1].emplace_front(v2, std::make_pair(w1, w2));
	  return true;
#endif
	}
      eb = ei;
    }
  return false;
}
