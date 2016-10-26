#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <cstring> // strcmp()
#include <iterator>
#include <limits>
#include <memory>
#include <mutex>
#include <condition_variable>

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include "DataElement.hpp"
#include "IE_types.hpp"

const size_t NUM_INSERTS = 100;
const size_t POOL_SIZE = sizeof(std::pair<int,std::pair<int,size_t>>) * NUM_INSERTS;

using namespace IE_types;

using VertexList = ForwardList<IPair, POOL_SIZE>;
using VertexIterator = VertexList::iterator;
using CVertexIterator = VertexList::const_iterator;

enum vertexColor { White, Gray, Black };
enum edgeType { Tree, Backward, Forward, Cross };

using SERIALIZER_W = boost::lock_guard<boost::shared_mutex>;
using SERIALIZER_R = boost::shared_lock<boost::shared_mutex>;

#define SERIALIZE_WRITES SERIALIZER_W lg(mut_);
#define SERIALIZE_READS SERIALIZER_R ls(mut_);


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
  Graph(const Graph&);
  Graph& operator=(const Graph&) = default;
  Graph(Graph&&) noexcept;
  Graph& operator=(Graph&&) noexcept;

  inline void accept(Visitor* v) override {
    // SERIALIZE_READS;
    v->visit(this);
  };

  inline const size_t numVertices() const { return n_; }
  inline bool directed() const { return directed_; }
  inline bool isEdge(int, int) const;
  inline bool isEdge(int, int, std::pair<int, size_t>&) const;
  inline int edgeWeight(int, int) const;

    void addEdge(int, int,  const std::pair<int,size_t>&);
    void addEdge(int, int);
  void addEdge(int, int, int, int);
  bool removeEdge(int, int);
  bool updateEdgeWeight(int, int, int, size_t);

  VertexIterator begin(int u) { return vertices_[u].begin(); }
  VertexIterator end(int u) { return vertices_[u].end(); }
    CVertexIterator cbegin(int u) { return vertices_[u].cbegin(); }
    CVertexIterator cend(int u) { return vertices_[u].cend(); }

  void load(std::string filename);			 

protected:
  mutable boost::shared_mutex mut_;
  int n_;
  bool directed_;
  std::vector<VertexList> vertices_;

};

#endif
