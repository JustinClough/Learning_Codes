#include "driver.hpp"

#include <iostream>
#include <cstdlib>

#include "mesh.hpp"

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
  Mesh *mesh = new Mesh( Np1);
  mesh->print_stats();


  delete mesh;

  return;
}



