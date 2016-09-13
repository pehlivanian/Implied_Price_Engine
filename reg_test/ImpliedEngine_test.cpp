#include "ImpliedEngine_test.hpp"

int main(int argc, char **argv)
{
  using IE8 = ImpliedEngine<8>;

  IE8* IE = new IE8;

  delete IE;
}
