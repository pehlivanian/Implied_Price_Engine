#include "SecPair_test.hpp"

int main(int argc, char **argv)
{
#if 1
  {
    SecPair p1(1,2,1);
    SecPair p2(2,5,1);
    SecPair p12(1,5,1);
    SecPair sum12 = p1 + p2;
    
    assert(sum12 == p12);
  }
#endif
#if 1
  {
    SecPair p1(4,8,1);
    SecPair p2(2,4,1);
    SecPair p12(2,8,1);
    SecPair sum12 = p1 + p2;
    
    assert(sum12 == p12);
  }
#endif
#if 1
  {
    SecPair p1(4,8,1);
    SecPair p2(4,10,-1);
    SecPair p12(8,10,-1);
    SecPair sum12 = p1 + p2;

    assert(sum12 == p12);
  }
#endif
#if 1
  {
    SecPair p1(2,8,-1);
    SecPair p2(8,10,-1);
    SecPair p12(2,10,-1);
    SecPair sum12 = p1 + p2;

    assert(sum12 == p12);
  }
#endif
#if 1
  {
    SecPair p1(3,8,-1);
    SecPair p2(0,3,-1);
    SecPair p12(0, 8,-1);
    SecPair sum12 = p1 + p2;

    assert(sum12 == p12);
  }
#endif
#if 1
  {
    SecPair p1(5, 12,1);
    SecPair p2(3,5, 1);
    SecPair p12(3,12,1);
    SecPair sum12 = p1 + p2;

    assert(sum12 == p12);
  }
#endif
#if 1
  {
    SecPair p1(5,12,1);
    SecPair p2(5,8, 1);
    SecPair p12(8,12,1);
    SecPair diff12 = p1 - p2;

    assert(diff12 == p12);
  }
#endif
#if 1
  {
    SecPair p1(5,12,1);
    SecPair p2(-1,-1, 1);
    SecPair p12(5,12,1);
    SecPair sum12 = p1 + p2;

    assert(sum12 == p12);
  }
#endif
#if 1
  {
    SecPair p1(5,7,1);
    SecPair p2(5,8,-1);
    SecPair p12(7,8,-1);
    SecPair p3(p2);
    SecPair p4 = p2;
    SecPair p5 = p1 + p2;

    assert(p5 == p12);
  }
#endif
#if 1
  {
    SecPair p1(8,9,1);
    SecPair p2(-1,-1,1);
    SecPair p12(8,9,1);
    SecPair sum12 = p1 + p2;

    assert(p12 == sum12);
  }
#endif
#if 1
  {
    SecPair p1(8,9,1);
    SecPair p2(-1,-1,1);
    SecPair p12(8,9,1);
    SecPair diff12 = p1 - p2;

    assert(p12 == diff12);
  }
#endif
#if 1
  {
    SecPair p1(1,2,1);
    SecPair p2(2,1,1);
    p1 *= -1;

    assert(p1 == p2);
  }
#endif

#if 1
  {
    SecPair p1(-1, -1, 1);
    SecPair p2(0, -1, 1);
    SecPair p12(0, -1, -1);
    SecPair pdiff = p1 - p2;
    assert(p12 == pdiff);
  }
#endif  
  return 0;
}
