#include "gentest.h"

int main(int argc, char **argv)
{
  struct timespec before, after;
  struct timeval beforeV, afterV;

  const int R = 25;
  const int C = 1000000;
  
  long* MSDtimes[R];
  long* NSDtimes[R];
  for(size_t i=0; i<R; ++i)
    {
      MSDtimes[i] = (long*)malloc(C * sizeof(long));
      NSDtimes[i] = (long*)malloc(C * sizeof(long));
    }
		   
  std::string workdir = "../graph_exs/CL_";
  
  for(int r=0; r<R; ++r)
    {
      if (r > 1)
	{
	  std::string filename = workdir + std::to_string(r) + ".dat";
	  char* fn = new char[filename.length() + 1];
	  std::strcpy(fn, filename.c_str());
	  
	  Graph* g = new MarketGraph(0);
	  g->load(fn);
	  
	  ::dijkstra_visitor* dv = new ::dijkstra_visitor();
	  
	  for(int c=0; c<C; ++c)
	    {
	      
	      gettimeofday(&beforeV, 0);
	      port_gettime(&before);	      
	      dv->visit(g);
	      gettimeofday(&afterV, 0);
	      port_gettime(&after);
	      
	      MSDtimes[r][c] = diffTimer(&beforeV, &afterV);
	      NSDtimes[r][c] = diffNanoTimer(&before, &after);	      	      
	    }
	}
      else
	{
	  for(int c=0; c<C; ++c)
	    {
	      MSDtimes[r][c] = 0.;
	      NSDtimes[r][c] = 0.;
	      
	    }
	}
    }
  
  printf ("Table (micros) for Calc Step\n");
  printf ("#legs\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(MSDtimes, R, C);
  
  printf ("Table (nanos) for Calc Step\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(NSDtimes, R, C);

  return 0;
}

