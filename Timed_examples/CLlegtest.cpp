#include "CLlegtest.h"

int main(int argc, char **argv)
{

  struct timespec before, after;
  struct timeval beforeV, afterV;

  const int R = 25;
  const int C = 1000;
  
  long* MSDtimes[R];
  long* NSDtimes[R];
  for(size_t i=0; i<R; ++i)
    {
      MSDtimes[i] = (long*)malloc(C * sizeof(long));
      NSDtimes[i] = (long*)malloc(C * sizeof(long));
    }
		   
  long* MSBFtimes[R];
  long* NSBFtimes[R];
  for(size_t i=0; i<R; ++i)
    {
      MSBFtimes[i] = (long*)malloc(C * sizeof(long));
      NSBFtimes[i] = (long*)malloc(C * sizeof(long));
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
	  
	  ::cat2_visitor* dv = new ::cat2_visitor();
	  ::cat1_visitor* bfv = new ::cat1_visitor();
	  
	  for(int c=0; c<C; ++c)
	    {
	      
	      gettimeofday(&beforeV, 0);
	      port_gettime(&before);	      
	      dv->visit(g);
	      gettimeofday(&afterV, 0);
	      port_gettime(&after);
	      
	      MSDtimes[r][c] = diffTimer(&beforeV, &afterV);
	      NSDtimes[r][c] = diffNanoTimer(&before, &after);
	      
	      
	      gettimeofday(&beforeV, 0);
	      port_gettime(&before);
	      bfv->visit(g);
	      gettimeofday(&afterV, 0);
	      port_gettime(&after);
	      
	      MSBFtimes[r][c] = diffTimer(&beforeV, &afterV);
	      NSBFtimes[r][c] = diffNanoTimer(&before, &after);
	      
	    }
	}
      else
	{
	  for(int c=0; c<C; ++c)
	    {
	      MSDtimes[r][c] = 0.;
	      NSDtimes[r][c] = 0.;
	      
	      MSBFtimes[r][c] = 0.;
	      NSBFtimes[r][c] = 0.;
	    }
	}
    }
  
  printf ("Table (micros) for Cat1 Algorithm\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(MSBFtimes, R, C);
  
  printf ("Table (nanos) for Cat1 Algorithm\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(NSBFtimes, R, C);

  printf ("Table (micros) for Cat2 Algorithm\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(MSDtimes, R, C);
  
  printf ("Table (nanos) for Cat2 Algorithm\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(NSDtimes, R, C);
  
  return 0;
}

