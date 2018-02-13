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

  return;
}

VectorXd forcingFactory::getForcing()
{
  return F;
}

void forcingFactory::assign_force( int row)
{
  double hi   = m->getElem( row).getLength();
  double hip1 = m->getElem( row + 1).getLength();

  double xim1 = m->getElem( row).getLeftPos();
  double xi   = m->getElem( row).getRightPos();
  double xip1 = m->getElem( row + 1).getRightPos();

  double alpha = 0.0;
  double beta  = 0.0;

  double p = 0.0;
  double q = 0.0;

  double fim1 = 0.0;
  double fi   = 0.0;
  double fip1 = 0.0;

  if ( caseNumber == 1)
  { // A caseNumber of 1 is for the p = 3, q = 2 problem
    p = 3.0;
    q = 2.0;

    fim1 = analytic_p3q2( xim1);
    fi   = analytic_p3q2( xi);
    fip1 = analytic_p3q2( xip1);
  }
  else if ( caseNumber == 2)
  { // A casenumber of 2 is for the p = 1+x, q=0 problem
    q = 0.0;

    fim1 = analytic_pXq0( xim1);
    fi   = analytic_pXq0( xi);
    fip1 = analytic_pXq0( xip1);
  }
  else if ( caseNumber == 3)
  { 
    p = 3.0;
    q = 2.0;
  
    alpha = 4.0;
    beta  = 4.0;

    fim1 = analytic_3( xim1);
    fi   = analytic_3( xi);
    fip1 = analytic_3( xip1);
  }
  else if ( caseNumber == 4)
  { 
    p = 3.0;
    q = 2.0;
  
    alpha = -2.0;
    beta  = -1.0;

    fim1 = analytic_4( xim1);
    fi   = analytic_4( xi);
    fip1 = analytic_4( xip1);
  }
  else if ( caseNumber == 5)
  { 
    p = 3.0;
    q = 2.0;
  
    alpha = -3.0;
    beta  = -2.0;

    fim1 = analytic_5( xim1);
    fi   = analytic_5( xi);
    fip1 = analytic_5( xip1);
  }
  else 
  {
    std::cout << "Unrecognized caseNumber = " << caseNumber << std::endl;
    std::abort();
  }


  if ( row == 0)
  {
    F( row) -= alpha * (- p / hi + q * hi / 6.0);
  }
  else if ( row == (F.size() - 1))
  {
    F( row) -= beta * (- p / hip1 + q * hip1 / 6.0);
  }

  F( row) += fim1 * hi / 6.0 
            + fi * ( hi + hip1) / 3.0 
            + fip1 * hip1 / 6.0;
  return;
}

double forcingFactory::analytic_3( double x)
{
  return 8.0;
}

double forcingFactory::analytic_4( double x)
{
  double f = 0.0;
  f = 2.0 * x - 4.0;
  return f;
}

double forcingFactory::analytic_5( double x)
{
  double f = 0.0;
  f = -6.0 + 2.0 * (x*x - 3.0);
  return f;
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
