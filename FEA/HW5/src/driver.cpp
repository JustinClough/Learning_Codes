#include "driver.hpp"

#include <iostream>

void drive_problem( int CaseNumber, int Np1)
{
  bool isL = false;

  mesh* m = new mesh( Np1, isL);

  m->print_mesh_stats();

  delete m;

}

