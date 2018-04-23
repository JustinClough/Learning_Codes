#include <iostream>

#include "driver.hpp"

int main( int argc, char** argv)
{
  int method = 0;

  check_inputs( argc, argv, &method);

  drive_problem( method);
  

  return 0;
}
