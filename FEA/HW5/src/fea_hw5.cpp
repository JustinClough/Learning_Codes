#include <iostream>
#include "driver.hpp"

int main( int argc, char** argv)
{
  seed_random();

  int CaseNumber = 0;
  //int Np1[2] = { 20, 40};

  int Np1[2] = { 3, 3};


  for( int i = 0; i < 2; i++)
  {
    drive_problem( CaseNumber, Np1[i]);  
  }

  return 0;
}
