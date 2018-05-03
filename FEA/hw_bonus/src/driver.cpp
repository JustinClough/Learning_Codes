#include "driver.hpp"

#include <iostream>
#include <cstdlib>

#include "mesh.hpp"
#include "solution.hpp"

void check_inputs( int     argc, 
                   char**  argv,
                   int*    pmeth)
{
  if ( argc != 2)
  {
    std::cout 
      << "Usage: hw6 <method>"
      << std::endl
      << "method = 0: Zero Left-DBC"
      << std::endl
      << "method = 1: Zero Average"
      << std::endl;

    std::abort();
  }

  *pmeth = std::atoi( argv[1]);

  if( *pmeth != 0 && *pmeth != 1)
  {
    std::cout 
      << "Unrecognized Method = "
      << *pmeth
      << std::endl;

    std::abort();
  }

  std::cout << "method = " << *pmeth << std::endl;
  return;
}

void drive( int Np1, int method, Printer* printer)
{
  Mesh* mesh = new Mesh( Np1);

  Solution* s = new Solution( mesh, printer, method);

  s->assemble_system();
  s->solve();
  s->calculate_errors();
  s->print_data();

  delete s;
  delete mesh;

  return;
}

void drive_problem( int method)
{
  Printer* printer = new Printer();

  int nN  = 9;
  int N   = 5;

  for( int i = 0; i < nN; i++)
  {
    drive( N, method, printer);
    N *= 2;
  }

  printer->print();

  delete printer;

  return;
}
