#include "mempooltest.h"

int main(int argc, char **argv)
{
  
  struct timespec before, after;
  struct timeval beforeV, afterV;

  const int R = 2;
  const int C = 1000;
  const int NUM_ALLOCS = 100;

  long* MStimes[R];
  long* NStimes[R];
  for(size_t i=0; i<R; ++i)
    {
      MStimes[i]  = (long*)malloc(C * sizeof(long));
      NStimes[i] = (long*)malloc(C * sizeof(long));
    }
  
  for(int r=0; r<R; ++r)
    {
      for(int c=0; c<C; ++c)
	{
	  if (r == 0)
	    {
	      gettimeofday(&beforeV, 0);
	      port_gettime(&before);
	      
	      for(int i=0; i<NUM_ALLOCS; ++i)
		mempooltester_stat* m = new mempooltester_stat(r);
	      
	      gettimeofday(&afterV, 0);
	      port_gettime(&after);
	    }
	  
	  else {
	    gettimeofday(&beforeV, 0);
	    port_gettime(&before);
	    
	    for(int i=0; i<NUM_ALLOCS; ++i)
	      mempooltester_dyn* m = new mempooltester_dyn(r);
	    
	    gettimeofday(&afterV, 0);
	    port_gettime(&after);
	  }
	  
	  MStimes[r][c] = diffTimer(&beforeV, &afterV);
	  NStimes[r][c] = diffNanoTimer(&before, &after);
	}
    }
  
  printf("Table (micros)\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(MStimes, R, C);

  printf("Table (nanos)\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(NStimes, R, C);    
  
  return 0;
}
