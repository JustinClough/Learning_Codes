#include <iostream>

#include "driver.hpp"

int main( int argc, char** argv)
{
  int cn  = 0;
  int np1 = 0;
  check_inputs( argc, argv, &cn, &np1);

  drive_problem( cn, np1);

  return 0;
}
