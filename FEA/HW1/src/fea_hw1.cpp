#include "driver.hpp"
#include <iostream>

int main( int argc, char** argv)
{
  // Define Mesh paramters, testing size
  double oddSize         = 0.9;
  double evenSize        = 1.1;
  int    NplusOneArray[] = {10, 20, 40, 80, 160, 320};
  //int    NplusOneArray[] = {10, 10, 10, 10, 10, 10};
  int    numTests        = 6;
  int    caseNumber = 1;

  drive_problem( oddSize, evenSize, NplusOneArray, numTests, caseNumber);

  return 0;
}
