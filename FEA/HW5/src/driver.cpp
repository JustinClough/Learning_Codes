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

  mesh* m  = new mesh( Np1, isL);
  mesh* pm = m->get_perturbed();
  m->print_mesh_stats();
  pm->print_mesh_stats();

  solution* s = new solution( m, CaseNumber);


  delete s;
  delete pm;
  delete m;

}

