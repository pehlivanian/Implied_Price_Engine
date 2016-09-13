#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <iostream>
#include <limits>
#include <forward_list>
#include <vector>
#include <string>
#include <cstring> // strcmp()
#include <iterator>
#include <limits>
#include <memory>

#include "DataElement.hpp"
#include "pool_allocator.hpp"

const size_t NUM_INSERTS = 100;
const size_t POOL_SIZE = sizeof(std::pair<int,int>) * NUM_INSERTS;

using IPair = std::pair<int, int>;
template<class T, std::size_t N> using A = stack_allocator<T, N>;
template<class T, std::size_t N> using ForwardList = std::forward_list<T, A<T,N>>;
using VertexList = ForwardList<IPair, POOL_SIZE>;
using VertexIterator = VertexList::const_iterator;

enum vertexColor { White, Gray, Black };
enum edgeType { Tree, Backward, Forward, Cross };

class Graph : public DataElement
{
public:
  using Graph_iterator = VertexList::const_iterator;
  Graph() : n_(0), directed_(true) 
  {
    arena<POOL_SIZE>* ar = new arena<POOL_SIZE>{};
    vertices_ = std::vector<VertexList>(1, VertexList{A<IPair,POOL_SIZE>(*ar)});
  }
  Graph(int n) : n_(n), directed_(true)
  {
    arena<POOL_SIZE>* ar = new arena<POOL_SIZE>{};
    vertices_ = std::vector<VertexList>(n, VertexList{A<IPair,POOL_SIZE>(*ar)});
  }
  Graph(int n, bool d) : n_(n), directed_(d) 
  {
    arena<POOL_SIZE>* ar = new arena<POOL_SIZE>{};
    vertices_ = std::vector<VertexList>(n, VertexList{A<IPair,POOL_SIZE>(*ar)});
  }
  Graph(const Graph&) = default;
  Graph& operator=(const Graph&) = default;
  Graph(Graph&&) noexcept;
  Graph& operator=(Graph&&) noexcept;
  
  inline const size_t numVertices() const { return n_; }
  inline bool directed() const { return directed_; }
  inline bool isEdge(int, int) const;
  inline bool isEdge(int, int, int &) const;
  inline int edgeWeight(int, int) const;

  void addEdge(int, int);
  void addEdge(int, int, int);
  bool removeEdge(int, int);
  bool updateEdgeWeight(int, int, int);

  // Utils
  VertexIterator begin(int u) const { return vertices_[u].begin(); }
  VertexIterator end(int u) const { return vertices_[u].end(); }

  void load(std::string filename);			 

protected:
  int n_;
  bool directed_;
  std::vector<VertexList> vertices_;

};

#endif
