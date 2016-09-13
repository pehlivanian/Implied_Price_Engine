#include "allocatortest.h"

template <class T, std::size_t N> using A = stack_allocator<T, N>;
template <class T, std::size_t N> using ForwardList = std::forward_list<T, A<T,N>>;


int main(int argc, char **argv)
{

  struct timespec before, after;
  struct timeval beforeV, afterV;

  const int R = 2;
  const int C = 10;
  const int NUM_INSERTS = 10000;
  const int NUM_SHIFTS = 5000;

  long* MStimes[R];
  long* NStimes[R];
  for(size_t i=0; i<R; ++i)
    {
      MStimes[i] = (long*)malloc(C * sizeof(long));
      NStimes[i] = (long*)malloc(C * sizeof(long));
    }

  std::default_random_engine e;
  std::uniform_int_distribution<int> d(0, NUM_INSERTS-1);
  auto randomizer = std::bind(d, e);

  const std::size_t N = NUM_INSERTS * sizeof(int);
  arena<N> a;
  ForwardList<int, N> F{A<int, N>(a)};
  std::forward_list<int> f;
  for(int i=0; i<NUM_INSERTS; ++i)
    {
      int dev = randomizer();
      F.push_front(dev);
      f.push_front(dev);
    }

  for(int r=0; r<R; ++r)
    {
      for(int c=0; c<C; ++c)
	{
	  if (r == 0)
	    {
	      gettimeofday(&beforeV, 0);
	      port_gettime(&before);
	      for(int i=0; i<NUM_INSERTS; ++i)
		{
		  int shifts = 0;
		  auto eb = F.begin();
		  while(shifts<NUM_SHIFTS) { ++eb; ++shifts; }
		  F.erase_after(eb);
		  F.emplace_front(i);
		}
	      gettimeofday(&afterV, 0);
	      port_gettime(&after);
	    }
	  else
	    {
	      gettimeofday(&beforeV, 0);
	      port_gettime(&before);
	      for(int i=0; i<NUM_INSERTS; ++i)
		{
		  int shifts = 0;
		  auto eb = f.begin();
		  while(shifts<NUM_SHIFTS) { ++eb; ++shifts; }
		  f.erase_after(eb);
		  f.emplace_front(i);
		}
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


