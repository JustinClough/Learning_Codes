#include "driver.hpp"
#include <iostream>

int main( int argc, char** argv)
{
  // Define Mesh paramters, testing size
  double oddSize         = 0.9;
  double evenSize        = 1.1;
  int    NplusOneArray[] = {10, 20, 40, 80, 160, 320};
  int    numTests        = 6;

  bool debug = false;

  if (debug)
  {
    oddSize         = 1.0;
    evenSize        = 1.0;
    for (int i = 0; i < numTests; i++)
    {
    NplusOneArray[i] = 3*(i+1);
    }
  }

  for ( int caseNumber = 1; caseNumber < 6; caseNumber++)
  {
    drive_problem( oddSize, evenSize, NplusOneArray, numTests, caseNumber);
  }

  return 0;
}
