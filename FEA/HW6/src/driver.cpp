#include "driver.hpp"

#include <iostream>
#include <cstdlib>

void check_inputs( int argc, char** argv, int* pcn, int* pnp1)
{
  if ( argc != 3)
  {
    std::cout 
      << "Usage: hw6 Case_Number N+1 "
      << std::endl;

    std::abort();
  }

  *pcn  = std::atoi( argv[1]);
  *pnp1 = std::atoi( argv[2]);

  std::cout << "cn = " << *pcn << std::endl;
  std::cout << "np1 = " << *pnp1 << std::endl;

  if( *pcn <= 0 || *pcn > 2)
  {
    std::cout 
      << "Unrecognized Case Number = "
      << *pcn 
      << std::endl;

    std::abort();
  }
  return;
}

void drive_problem( int CaseNumber, int Np1)
{
  std::cout
    << "CaseNumber = " << CaseNumber
    << std::endl;

  std::cout
    << "Np1 = " << Np1
    << std::endl;

  return;
}



