#include "stiffness.hpp"

#include <iostream>

MatrixXd create_stiffness( mesh1D* mesh, int caseNumber)
{
  int numDofs = mesh->getNumNodes();
  MatrixXd K (numDofs, numDofs);
  K = MatrixXd::Zero( numDofs, numDofs);

  std::cout << "K = " << K << std::endl;
  return K;
}
