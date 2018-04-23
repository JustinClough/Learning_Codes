#include "driver.hpp"

#include <iostream>
#include <cstdlib>

#include "mesh.hpp"
#include "solution.hpp"

void check_inputs( int     argc, 
                   char**  argv,
                   int*    pnp1,
                   int*    pmeth)
{
  if ( argc != 3)
  {
    std::cout 
      << "Usage: hw6 <N+1> <method>"
      << std::endl
      << "method = 0: Zero Left-DBC"
      << std::endl
      << "method = 1: Zero Average"
      << std::endl;

    std::abort();
  }

  *pnp1  = std::atoi( argv[1]);
  *pmeth = std::atoi( argv[2]);

  if( *pmeth != 0 && *pmeth != 1)
  {
    std::cout 
      << "Unrecognized Method = "
      << *pmeth
      << std::endl;

    std::abort();
  }

  if( *pnp1 <= 0)
  {
    std::cout 
      << "Impossible Number of Nodes = "
      << *pnp1
      << std::endl;

    std::abort();

  }

  std::cout << "np1 = " << *pnp1 << std::endl;
  std::cout << "method = " << *pmeth << std::endl;
  return;
}

void drive_problem( int Np1, 
                    int method)
{
  Mesh* mesh = new Mesh( Np1);
  mesh->print_stats();

  Printer* printer = new Printer();

  Solution* s = new Solution( mesh, printer, method);

  s->assemble_system();
  s->solve();
  s->calculate_errors();
  s->print_data();

  delete printer;
  delete s;
  delete mesh;

  return;
}



