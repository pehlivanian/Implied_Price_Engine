#include "grouptest.h"

int main(int argc, char **argv)
{

  long len;
  int i, x;

  int i1=0;
  long min   =  8000000;
  long max   = 16000000;
  long delta =  2000000; 

  struct timespec before, after;
  struct timeval beforeV, afterV;

  const int R = 5;
  const int C = 10;
  long* MStimes[R];
  long* NStimes[R];
  for(size_t i=0; i<R; ++i)
    {
      MStimes[i] = (long*)malloc(C * sizeof(long));
      NStimes[i] = (long*)malloc(C * sizeof(long));
    }

  for (len = min; len <= max; len += delta, i1++) {
    for (i = 0; i < C; i++) {
      long sum;

      gettimeofday(&beforeV, 0);    /* begin time */
      port_gettime(&before);

      sum = 0;
      for (x = 0; x < len; x++) { sum += x; }

      gettimeofday(&afterV, 0);    /* begin time */
      port_gettime(&after);

      /** Without this, loop is optimized away. */
      if (sum == 1) {
	printf ("%ld\n", sum);
      }

      MStimes[i1][i] = diffTimer (&beforeV, &afterV);
      NStimes[i1][i] = diffNanoTimer (&before, &after);
    }
  }

  printf ("Table for Grouptest\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(MStimes, R, C);

  printf ("Table for Grouptest\n");
  printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
  buildTable(NStimes, R, C);

  return 0;
}
