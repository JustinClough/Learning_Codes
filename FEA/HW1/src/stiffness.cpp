#include "stiffness.hpp"

#include <iostream>


springFactory::springFactory( mesh1D* mesh, int caseNumber_)
{
  m = mesh;
  caseNumber = caseNumber_;
}

void springFactory::create_stiffness()
{
  int numDofs = m->getNumNodes();
  MatrixXd K (numDofs, numDofs);
  K = MatrixXd::Zero( numDofs, numDofs);

  for( int i = 0; i < numDofs; i++)
  {
    for( int j = 0; j < numDofs; j++)
    {
      assign_stiffness( i, j);
    }
  }

  std::cout << "K = " << K << std::endl;
  return ;
}

void springFactory::assign_stiffness( int row, int col)
{
  if ( caseNumber == 1)
  { // A caseNumber of 1 is for the p = 3, q = 2 problem
    p3q2Stiffness( row, col);
  }
  else if ( caseNumber == 2)
  { // A casenumber of 2 is for the p = 1+x, q=0 problem
    pXq0Stiffness( row, col);
  }
  else 
  {
    std::cout << "Unrecognized caseNumber = " << caseNumber << std::endl;
  }
  return;
}

MatrixXd springFactory::getStiffness()
{
  return K;
}

void springFactory::p3q2Stiffness( int row, int col)
{
  // TODO
  return;
}

void springFactory::pXq0Stiffness( int row, int col)
{
  // TODO
  return;
}
