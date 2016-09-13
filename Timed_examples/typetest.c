#include "typetest.h"

int main(int argc, char **argv)
{
  struct timespec before, after;
  struct timeval beforeV, afterV;

  const int NUM_ITS = 100000;

  const int R = 1;
  const int C = NUM_ITS;
  const short  LARGE_NUMs = 1 << 4;
  const int    LARGE_NUMi = 1 << 4;
  const float  LARGE_NUMf = static_cast<float>(1 << 4);
  const double LARGE_NUMd = static_cast<double>(1 << 4);
  
  long* MStimes[R];
  long* NStimes[R];
  for(size_t i=0; i<R; ++i)
    {
      MStimes[i] = (long*)malloc(C * sizeof(long));
      NStimes[i] = (long*)malloc(C * sizeof(long));
    }

  short    xs = 0, rs = 0;
  int      xi = 0, ri = 0;
  float    xf = 0, rf = 0;
  double   xd = 0, rd = 0;
  
  for(size_t c=0; c<C; ++c)
    {
      
      gettimeofday(&beforeV, 0);
      port_gettime(&before);
      
      for(int i=0; i<LARGE_NUMi; ++i)
	{
	  xd = xd + 1.;
	  rd = rd + xd - 2.33*xd*xd;
	  if (rd > LARGE_NUMd)
	    rd = 0;
	}

      gettimeofday(&afterV, 0);
      port_gettime(&after);

      MStimes[0][c] = diffTimer(&beforeV, &afterV);
      NStimes[0][c] = diffNanoTimer(&before, &after);

    }
  
  // So that the loop doesn't get optimized out
  printf("DOUBLE result: %lf\n", rd);
  
  
  printf ("Table (micros) for Typetest\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(MStimes, R, C);
  
  printf ("Table (nanos) for Typetest\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(NStimes, R, C);
    
  return 0;
}
