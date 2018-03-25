#include "driver.hpp"

#include <iostream>

void drive_problem()
{

  mesh* m = new mesh( 4, false);

  m->print_mesh_stats();

  delete m;

}

