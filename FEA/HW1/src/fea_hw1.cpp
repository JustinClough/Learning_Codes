#include "eig_wrap.hpp"
#include "mesh1D.hpp"

#include <iostream>

int main( int argc, char** argv)
{

  double oddSize  = 0.9;
  double evenSize = 1.1;
  int    NplusOne = 10; 
  mesh1D mesh( oddSize, evenSize, NplusOne);

  return 0;
}
