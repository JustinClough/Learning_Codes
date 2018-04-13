#include <iostream>

#include "driver.hpp"

int main( int argc, char** argv)
{
  int cn     = 0;
  int np1    = 0;
  int method = 0;
  double dt  = 0.0;

  check_inputs( argc, argv, &cn, &np1, &method, &dt);

  drive_problem( cn, np1, method, dt);

  return 0;
}
