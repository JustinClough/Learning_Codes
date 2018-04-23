#include <iostream>

#include "driver.hpp"

int main( int argc, char** argv)
{

  int np1    = 0;
  int method = 0;

  check_inputs( argc, argv, &np1, &method);

  drive_problem( np1, method);

  return 0;
}
