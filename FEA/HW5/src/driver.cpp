#include "driver.hpp"

#include <iostream>

#include "solution.hpp"
#include "mesh.hpp"

void seed_random()
{
  srand( time( NULL));
  return;
}

void drive_problem( int CaseNumber, int Np1)
{
  bool isL = false;
  if( CaseNumber == 5)
  {
    isL = true;
  }
  
  bool isCurved = false;
  if( CaseNumber == 6)
  {
    isCurved = true;
  }

  mesh* m  = new mesh( Np1, isL, isCurved);
  
  std::cout 
    << std::endl
    << "Regular mesh"
    << std::endl;

  solution* s = new solution( m, CaseNumber);
  s->assemble_problem();
  s->apply_boundary_conditions();
  s->solve_system();
  s->compute_errors();
  delete s;

  if( CaseNumber <= 4)
  {
    mesh* pm = m->get_perturbed();

    std::cout 
      << std::endl
      << "Peturbed mesh"
      << std::endl;

    solution* sp = new solution( pm, CaseNumber);
    sp->assemble_problem();
    sp->apply_boundary_conditions();
    sp->solve_system();
    sp->compute_errors();
    delete sp;

    delete pm;
  }

  delete m;

  return;
}
