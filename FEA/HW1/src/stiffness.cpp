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
    std::abort();
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
  double hi   = m->getElem( row).getLength();
  double hip1 = m->getElem( row + 1).getLength();

  double xim1 = m->getElem( row).getLeftPos();
  double xi   = m->getElem( row).getRightPos();
  double xip1 = m->getElem( row).getRightPos();

  if( row == col)
  {
    // On the diagonal
    double tmp1 = ( xi * xi - xim1 * xim1) / 2.0;
    tmp1 += hi;
    tmp1 /= (hi * hi);
    double tmp2 = ( xip1 * xip1 - xi * xi) / 2.0;
    tmp2 += hip1;
    tmp2 /= (hip1 * hip1);
    K( row, col) = tmp1 + tmp2;
  }
  else if ( std::abs( row - col) == 1.0)
  {
    if ( (row - col) < 0.0)
    {
      // Inside of diagonal
      double tmp = ( xi * xi - xim1 * xim1) / 2.0;
      tmp += hi;
      tmp /= -1.0 * (hi * hi);
      K( row, col) = tmp;
    }
    else if ( (row - col) > 0.0)
    {
      // Inside of diagonal
      double tmp = ( xip1 * xip1 - xi * xi) / 2.0;
      tmp += hip1;
      tmp /= -1.0 * (hip1 * hip1);
      K( row, col) = tmp;
    }
  }
  return;
}
