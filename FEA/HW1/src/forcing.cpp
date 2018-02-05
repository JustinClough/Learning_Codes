#include "forcing.hpp"


forcingFactory::forcingFactory( mesh1D* mesh, int caseNumber_)
{
  m = mesh;
  caseNumber = caseNumber_;
  int numNodes = m->getNumNodes();
  // Assuming that only the interior nodes are
  //  degrees of freedom
  numDofs = numNodes - 2;
  F = VectorXd::Zero( numDofs);
}

forcingFactory::create_forcing()
{
  for ( int i = 0; i < numDofs; i++)
  {
    assign_force( i);
  }

  std::cout << "F = " << std::endl;
  std::cout << F << std::endl;

  return;
}

VectorXd forcingFatory::getForcing()
{
  return F;
}


