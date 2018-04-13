#include "driver.hpp"

#include <iostream>
#include <cstdlib>

#include "mesh.hpp"
#include "solution.hpp"

void check_inputs( int     argc, 
                   char**  argv,
                   int*    pcn,
                   int*    pnp1,
                   int*    pmeth,
                   double* pdt)
{
  if ( argc != 5)
  {
    std::cout 
      << "Usage: hw6 <Case_Number> <N+1> <method> <dt>"
      << std::endl
      << "Case_Number = 1: u_0(x) = sin( 2x)"
      << std::endl
      << "Case_Number = 2: u_0(x) = x *jump* x-pi"
      << std::endl
      << "method = 1: Forward Euler"
      << std::endl
      << "method = 2: Backward Euler"
      << std::endl
      << "method = 3: Crank-Nicolson"
      << std::endl;

    std::abort();
  }

  *pcn   = std::atoi( argv[1]);
  *pnp1  = std::atoi( argv[2]);
  *pmeth = std::atoi( argv[3]);
  *pdt   = std::atof( argv[4]);

  if( *pcn <= 0 || *pcn > 2)
  {
    std::cout 
      << "Unrecognized Case Number = "
      << *pcn 
      << std::endl;

    std::abort();
  }

  if( *pmeth <= 0 || *pmeth > 3)
  {
    std::cout 
      << "Unrecognized Method = "
      << *pmeth
      << std::endl;

    std::abort();
  }

  if( *pdt <= 0.0)
  {
    std::cout 
      << "Impossible Time Step = "
      << *pdt
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

  std::cout << "cn = " << *pcn << std::endl;
  std::cout << "np1 = " << *pnp1 << std::endl;
  std::cout << "method = " << *pmeth << std::endl;
  std::cout << "dt = " << *pdt << std::endl;
  return;
}

void drive_problem( int CaseNumber, 
                    int Np1, 
                    int method, 
                    double dt)
{
  Mesh *mesh = new Mesh( Np1);
  mesh->print_stats();

  Solution* s = new Solution( mesh, CaseNumber, method, dt);

  s->assemble_system();
  s->assign_boundary_conditions();

  double T_end = 1.0;
  s->solve( T_end);
  s->calculate_errors();
  s->print_data();

  T_end = 10.0;
  s->solve( T_end);
  s->calculate_errors();
  s->print_data();

  delete s;
  delete mesh;

  return;
}



