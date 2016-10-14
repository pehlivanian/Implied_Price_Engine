#ifndef __GRAPH_UTILS_HPP__
#define __GRAPH_UTILS_HPP__

#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <string>
#include <fstream>
#include <cassert>

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/edge_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

#include "Graph.hpp"
#include "MarketGraph.hpp"


using namespace boost;

using Edge = std::pair<int,int>;
using EdgeList = std::vector<Edge>;
using graph_t = boost::adjacency_list<listS, vecS, directedS,
				      no_property, property<edge_weight_t, int>>;
using Vertex = graph_traits<graph_t>::vertex_descriptor;
using vIt = graph_traits<graph_t>::vertex_iterator;
using Vertexcont = std::list<Vertex>;
using vContIt = Vertexcont::const_iterator;
using outIt = graph_traits<graph_t>::out_edge_iterator;
using inIt = graph_traits<graph_t>::in_edge_iterator;

class graph_utils
{
public:
  static graph_t loadBoostGraph(char *filename)
  {
  FILE *fp = fopen (filename, "r");
  int nv, ne;
  std::vector<Edge> edges;
  std::vector<int> weights;

  // sufficient space? we hope so!
  char buf[4096];

  // read header as a whole line.
  fgets (buf, 4096, fp);
  
  // read n and e
  int nr = fscanf (fp, "%d %d %s\n", &nv, &ne, buf);
  if (nr != 3) {
    printf ("Invalid file format %s\n", filename);
    exit(1);
  }
  
  edges.resize(ne);
  weights.resize(ne);
  int edge_count = 0;

  while(ne-- > 0)
    {
      int src, tgt, weight;
      int nr = fscanf(fp, "%d,%d,%d\n", &src, &tgt, &weight);
      assert(nr > 0);
      edges[edge_count] = std::make_pair(src, tgt);
      weights[edge_count] = weight;
      edge_count++;
    }

  fclose(fp);

  int num_arcs = edges.size();
  graph_t g = graph_t(&edges[0], &edges[0] + num_arcs, &weights[0], nv);

  return g;

  }

  static void toDot(MarketGraph *g, std::string filename, 
		    const std::vector<int>& d=std::vector<int>{0}, 
		    const std::vector<int>& p=std::vector<int>{0})
  {
    std::ofstream dot_file(filename.c_str());

    dot_file << "digraph D {\n"
	     << "  rankdir=LR\n"
	     << "  size=\"4,3\"\n"
	     << "  ratio=\"fill\"\n"
	     << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

    dot_file << "{ rank = same; " << g->vertexProp(0) << "; }\n";
    dot_file << "{ rank = same; ";
    for(size_t i=2; i<g->numVertices(); ++i)
      dot_file << g->vertexProp(i) << ";";
    dot_file << "}\n";
    dot_file << "{ rank = same; " << g->vertexProp(1) << "; }\n";
    dot_file << g->vertexProp(0) << " [shape=box];\n";
    dot_file << g->vertexProp(1) << " [shape=box];\n";

    for (size_t i=0; i<g->numVertices(); ++i)
      {
	Graph::Graph_iterator gb = g->begin(i);
	Graph::Graph_iterator ge = g->end(i);
	while( gb != ge)
	  {
	    int u = i, v = gb->first, wt = (gb->second).first, sz = (gb->second).second;
	    dot_file << g->vertexProp(u) << " -> " << g->vertexProp(v)
		     << "[label=\"" << g->edgeProp(u,v)  << " : " << wt << "\"";
	    if ((p.size() > 1) && (p[v] == u))
	      dot_file << ", color=\"black\"";
	    else
	      dot_file << ", color=\"grey\"";
	    dot_file << "];\n";
	    ++gb;
	  }
      }
    dot_file << "}";
    
  }

  static void printPath(Graph *g, std::string filename,
			std::vector<int>& d, std::vector<int>& p)
  {
    std::vector<std::string> name;

    const size_t nv = g->numVertices();
    name.resize(nv);
    for(size_t i=0; i<nv; ++i)
      name[i] = std::to_string(i);

    std::cout << "distances and parents:" << std::endl;
    std::vector<int>::iterator pbeg = p.begin(), pend = p.end(), 
      dbeg = d.begin(), dend = d.end();
    
    for(;
	pbeg != pend; ++pbeg, ++dbeg)
      {
	std::cout << "distance(" << name[std::distance(p.begin(), pbeg)] 
		  << ") = " << *dbeg << ", ";
	std::cout << "parent(" << name[std::distance(p.begin(), pbeg)] 
		  << ") = " << *pbeg << std::endl;
      }
    std::cout << std::endl;

    for (size_t i=0; i<g->numVertices(); ++i)
      {
	Graph::Graph_iterator gb = g->begin(i);
	Graph::Graph_iterator ge = g->end(i);
	while( gb != ge)
	  {
	    std::cout << "Edge: " << i << " : " << (gb->second).first
		      << " : " << (gb->second).second << "\n";
	    ++gb;
	  }
      }

    toDot(dynamic_cast<MarketGraph*>(g), filename, d, p);

  }

  static void printPath(graph_t g, const std::vector<int>& d,
			std::vector<long unsigned int>& p)
  {
    property_map<graph_t, edge_weight_t>::type weightmap = 
      boost::get(edge_weight, g);
    
    std::vector<std::string> name;
    
    const size_t nv = num_vertices(g);
    name.resize(nv);

    for(size_t i=0; i<nv; ++i)
      name[i] = std::to_string(i);
    
    std::cout << "distances and parents:" << std::endl;
    graph_traits < graph_t >::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
      std::cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
      std::cout << "parent(" << name[*vi] << ") = " << name[p[*vi]] << std::
	endl;
    }
    std::cout << std::endl;
    
    std::ofstream dot_file("figs/dijkstra-eg.dot");
    
    dot_file << "digraph D {\n"
	     << "  rankdir=LR\n"
	     << "  size=\"4,3\"\n"
	     << "  ratio=\"fill\"\n"
	     << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";
    
    graph_traits < graph_t >::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
      graph_traits < graph_t >::edge_descriptor e = *ei;
      graph_traits < graph_t >::vertex_descriptor
	u = source(e, g), v = target(e, g);
      dot_file << name[u] << " -> " << name[v]
	       << "[label=\"" << get(weightmap, e) << "\"";
      if (p[v] == u)
	dot_file << ", color=\"black\"";
      else
	dot_file << ", color=\"grey\"";
      dot_file << "]";
    }
    dot_file << "}";
  }
};

#endif

