#include "cat2_visitortest.h"

int main(int argc, char **argv)
{
  struct timespec before, after;
  struct timeval beforeV, afterV;

  const int R = 2;
  const int C = 100;
  
  long* MStimes[R];
  long* NStimes[R];
  for(size_t i=0; i<R; ++i)
    {
      MStimes[i] = (long*)malloc(C * sizeof(long));
      NStimes[i] = (long*)malloc(C * sizeof(long));
    }

  // Evaluate 2 versions of cat2 algorithm
  std::string workdir = "../graph_exs/";

  for(size_t c=0; c<C; ++c)
    {      
      // First generate a random graph from Python call
      
      std::string filename = workdir + "random.dat";
      char* fn = new char[filename.length() + 1];
      std::strcpy(fn, filename.c_str());
      
      // Boost calculation
      graph_t g_boost = graph_utils::loadBoostGraph(fn);
      const int N = num_vertices(g_boost);
      std::vector<Vertex> p(N);
      std::vector<int> d(N);
      Vertex s = vertex(0, g_boost);
      
      gettimeofday(&beforeV, 0);
      port_gettime(&before);
      
      dijkstra_shortest_paths(g_boost, s,
                          predecessor_map(boost::make_iterator_property_map(p.begin(), 
			  get(boost::vertex_index, g_boost))).distance_map(
			  boost::make_iterator_property_map(d.
			  begin(), get(boost::vertex_index, g_boost))));

      gettimeofday(&afterV, 0);
      port_gettime(&after);
      
      MStimes[0][c] = diffTimer(&beforeV, &afterV);
      NStimes[0][c] = diffNanoTimer(&before, &after);
		 
      // Home-grown calculation
      Graph* g_mine = new Graph(0);
      g_mine->load(fn);
      ::cat2_visitor* dv = new ::cat2_visitor();

      gettimeofday(&beforeV, 0);
      port_gettime(&before);
      
      dv->visit(g_mine);
      gettimeofday(&afterV, 0);
      port_gettime(&after);
      
      MStimes[1][c] = diffTimer(&beforeV, &afterV);
      NStimes[1][c] = diffNanoTimer(&before, &after);
    }
  
  printf ("Table for Cat2 Test\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(MStimes, R, C);
  
  printf ("Table for Cat2 Test\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(NStimes, R, C);
    
  return 0;
}
