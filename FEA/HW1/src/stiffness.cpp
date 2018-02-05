#include "stiffness.hpp"

#include <iostream>
#include <cmath>

springFactory::springFactory( mesh1D* mesh, int caseNumber_)
{
  m = mesh;
  caseNumber = caseNumber_;
  int numNodes = m->getNumNodes();
  // Assuming that only the interior nodes are
  //  degrees of freedom
  numDofs = numNodes - 2;
  K = MatrixXd::Zero( numDofs, numDofs);
}

void springFactory::create_stiffness()
{
  for( int i = 0; i < numDofs; i++)
  {
    for( int j = 0; j < numDofs; j++)
    {
      assign_stiffness( i, j);
    }
  }

  std::cout << "K = " << std::endl;
  std::cout << K << std::endl;
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
  double p  = 3.0;
  double q  = 2.0;

  double hi   = m->getElem( row).getLength();
  double hip1 = m->getElem( row + 1).getLength();

  if( row == col)
  {
    // On the diagonal
    K( row, col) = p * (1.0/hi + 1.0/hip1) + q/3.0 * (hi + hip1);
  }
  else if ( std::abs( row - col) == 1.0)
  {
    if ( (row - col) < 0.0)
    {
      // Inside of diagonal
      K( row, col) = - p / hi + q * hi / 6.0;
    }
    else if ( (row - col) > 0.0)
    {
      // Inside of diagonal
      K( row, col) = - p / hip1 + q * hip1 / 6.0;
    }
  }
  return;
}

void springFactory::pXq0Stiffness( int row, int col)
{
  // TODO
  return;
}
