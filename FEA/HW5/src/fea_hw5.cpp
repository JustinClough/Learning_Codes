#include <iostream>
#include <cstdlib>
#include "driver.hpp"

int main( int argc, char** argv)
{
  if ( argc != 3)
  {
    std::cout 
      << "Usage: hw5 Case_Number N+1 "
      << std::endl;

    std::abort();
  }

  int cn  = std::atoi( argv[1]);
  int np1 = std::atoi( argv[2]);

  std::cout << "cn = " << cn << std::endl;
  std::cout << "np1 = " << np1 << std::endl;

  if( cn <= 0 || cn > 4)
  {
    std::cout 
      << "Unrecognized Case Number = "
      << cn 
      << std::endl;

    std::abort();
  }

  seed_random();

  drive_problem( cn, np1);  

  return 0;
}
