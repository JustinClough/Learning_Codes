#include "forcing.hpp"

#include <cmath>
#include <iostream>

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

void forcingFactory::create_forcing()
{
  for ( int i = 0; i < numDofs; i++)
  {
    assign_force( i);
  }

  std::cout << "F = " << std::endl;
  std::cout << F << std::endl;

  return;
}

VectorXd forcingFactory::getForcing()
{
  return F;
}

void forcingFactory::assign_force( int row)
{
  if ( caseNumber == 1)
  { // A caseNumber of 1 is for the p = 3, q = 2 problem
    p3q2force( row);
  }
  else if ( caseNumber == 2)
  { // A casenumber of 2 is for the p = 1+x, q=0 problem
    pXq0force( row);
  }
  else 
  {
    std::cout << "Unrecognized caseNumber = " << caseNumber << std::endl;
    std::abort();
  }
  return;
}

void forcingFactory::p3q2force( int row)
{
  double hi   = m->getElem( row).getLength();
  double hip1 = m->getElem( row + 1).getLength();

  double xim1 = m->getElem( row).getLeftPos();
  double xi   = m->getElem( row).getRightPos();
  double xip1 = m->getElem( row + 1).getLeftPos();

  double fim1 = analytic_p3q2( xim1);
  double fi   = analytic_p3q2( xi);
  double fip1 = analytic_p3q2( xip1);

  F( row) = fim1 * hi / 6.0 
            + fi * ( hi + hip1) / 3.0 
            + fip1 * hip1 / 6.0;

  return;
}

void forcingFactory::pXq0force( int row)
{
  double hi   = m->getElem( row).getLength();
  double hip1 = m->getElem( row + 1).getLength();

  double xim1 = m->getElem( row).getLeftPos();
  double xi   = m->getElem( row).getRightPos();
  double xip1 = m->getElem( row + 1).getLeftPos();

  double fim1 = analytic_pXq0( xim1);
  double fi   = analytic_pXq0( xi);
  double fip1 = analytic_pXq0( xip1);

  F( row) = fim1 * hi / 6.0 
            + fi * ( hi + hip1) / 3.0 
            + fip1 * hip1 / 6.0;
  return;
}

double forcingFactory::analytic_pXq0( double x)
{
  double f = 0.0;
  f += std::sin( 5.0 * x) * ( -9.0 * x - 1.0);
  f += std::cos( 5.0 * x) * ( -17.0 * x*x - x + 6.0);
  f += std::exp( x) * (- 3.0 * x*x*x - 15.0 * x*x - 15.0 * x +6.0);
  return f;
}

double forcingFactory::analytic_p3q2( double x)
{
  double f = 0.0;
  f += (17.0 * x * (x-1.0) - 6.0) * std::sin( 5.0 * x);
  f += -3.0 * x * (x + 11) * std::exp( x);
  f += -18.0 * (2.0 * x - 1) * cos( 5.0 * x);
  return f;
}
