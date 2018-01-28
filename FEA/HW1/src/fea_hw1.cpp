#include "eig_wrap.hpp"
#include <iostream>

int main( int argc, char** argv)
{

  MatrixXd m(2,2);
  m(0,0) = 1;
  m(1,0) = 2;
  m(0,1) = 3;
  m(1,1) = 4;

  std::cout << "m= " << m << std::endl;

  return 0;
}
