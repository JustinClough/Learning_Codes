#include "errorCalcs.hpp"

#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>

void errorCalcs( std::vector<VectorXd> solutions,
                 std::vector<mesh1D*> meshes, 
                 int caseNumber)
{
  EC* ec = new EC( solutions, meshes, caseNumber);

  ec->calculate_errors();
  ec->calculate_norms();
  ec->calculate_orders();
  ec->write_errors();

  delete ec;
  return;
}

EC::EC( std::vector<VectorXd> solutions,
        std::vector<mesh1D*> meshes,
        int caseNumber)
{
  sols = solutions;
  ms   = meshes;
  cn   = caseNumber;
}

void EC::calculate_errors()
{
  int M = 3;
  for( int i = 0; i < ms.size(); i++)
  {
    mesh1D*  mesh    = ms[i];
    VectorXd sol     = sols[i];

    int      numDofs = mesh->getNumNodes() - 1;
    VectorXd e       = VectorXd::Zero( M * numDofs);
    VectorXd dedx    = VectorXd::Zero( M * numDofs);
    VectorXd ePos    = VectorXd::Zero( M * numDofs);

    // Calculate the position error vector
    error_positions( ePos, mesh, M);

    // Calc the error vector at each position (including interpolation)
    eval_errors( e, ePos, sol, mesh, M);
    eval_ders( e, dedx, ePos, sol, mesh, M);

    es.push_back( e);
    xE.push_back( ePos);
    edxs.push_back( dedx);
  }
  return;
}

void EC::eval_ders( VectorXd& e, 
                    VectorXd& dedx, 
                    VectorXd& ePos, 
                    VectorXd& sol, 
                    mesh1D*   mesh,
                    int       M)
{
  for( int i = 0; i < sol.rows(); i++)
  {
    double uim1 = 0.0;
    double ui = 0.0;
    if( i == 0)
    {
      // Assuming the boundary is enforced
      uim1 = get_analytic( 0.0);
    }
    else
    {
      uim1 = sol( i - 1);
    }

    if( i == (e.rows() - 1))
    {
      // Assuming the boundary is enforced
      ui = get_analytic( 1.0);
    }
    else
    {
      ui = sol( i);
    }

    elem   element = mesh->getElem( i);
    double hi      = element.getLength();
    double duI     = (ui - uim1) / hi;

    double derror = 0.0;
    for( int j = 0; j < M; j++)
    {
      int    m     = j + 1;
      int    index = (i * M) + j;

      double xj  = ePos( index);
      double du  = get_analytic_der( xj);

      derror       = duI - du;
      dedx( index) = derror;
    }
  }
  return;
}

double EC::get_analytic_der( double x)
{
  double du = 0.0;
  if( cn == 1 || cn == 2)
  {
    double S = std::sin( 5.0 * x);
    double C = std::cos( 5.0 * x);
    double E = std::exp( x);
    du = ( x - 1.0) * ( S + 3.0 * E) 
       + x * (S + 3.0 * E) 
       + x * (x - 1.0) * ( 5.0 * C + 3.0 * E);
  }
  else if ( cn == 3)
  {
    du = 0.0;
  }
  else if ( cn == 4)
  {
    du = 1.0;
  }
  else if ( cn == 5)
  {
    du =  2.0 * x;
  }
  else
  {
    std::cout << "Bad case number of " << cn 
              << ". Aborting..." << std::endl;
    std::abort();
  }

  return du;
}

void EC::eval_errors( VectorXd& e, 
                      VectorXd& ePos, 
                      VectorXd& sol, 
                      mesh1D*   mesh,
                      int       M)
{
  int numElems = mesh->getNumElems();
  int numNodes = mesh->getNumNodes();
  for( int i = 0; i < numElems; i++)
  {
    double uim1 = 0.0;
    double ui = 0.0;
    if( i == 0)
    {
      // Assuming the boundary is enforced
      uim1 = get_analytic( 0.0);
    }
    else
    {
      uim1 = sol( i - 1);
    }

    if( i == (numElems - 1))
    {
      // Assuming the boundary is enforced
      ui = get_analytic( 1.0);
    }
    else
    {
      ui = sol( i);
    }

    elem element = mesh->getElem( i);
    double error = 0.0;
    for( int j = 0; j < M; j++)
    {
      int    m     = j + 1;
      int    index = (i * M) + j;

      double xj = ePos( index);
      double U  = get_analytic( xj);
      double UI = get_interp( uim1, ui, element, xj);

      error     = UI - U;
      e( index) = error;
    }
  }

  return;
}

double EC::get_interp( double ua, double ub, elem element, double x)
{
  double h   = element.getLength();
  double xa  = element.getLeftPos();
  double xb  = element.getRightPos();

  if ( (x < xa) || (xb < x) )
  {
    std::cout << "Interpolating outside of element!" << std::endl
              << "xa = " << xa << std::endl
              << "xb = " << xb << std::endl
              << "x = "  << x  << std::endl;
  }

  double uIa = ua * (xb - x) / h;
  double uIb = ub * (x - xa) / h;

  double uI  = uIa + uIb;
  return uI;
}

double EC::get_analytic( double x)
{
  double U = 0.0;
  if( cn == 1 || cn == 2)
  {
    U = x * (x - 1.0) * (std::sin( 5.0 * x) + 3.0 * std::exp( x));
  }
  else if ( cn == 3)
  {
    U = 4.0;
  }
  else if ( cn == 4)
  {
    U = x - 2.0;
  }
  else if ( cn == 5)
  {
    U = (x * x) - 3.0;
  }
  else
  {
    std::cout << "Bad case number of " << cn 
              << ". Aborting..." << std::endl;
    std::abort();
  }

  return U;
}

void EC::error_positions( VectorXd& ePos, mesh1D* mesh, int M)
{
  int numElems = mesh->getNumElems();
  for( int i = 0; i < numElems; i++)
  {
    elem   element = mesh->getElem( i);
    double xim1    = element.getLeftPos();
    double hi      = element.getLength();

    double m  = 0.0;
    double MM = 3.0;
    for( int j = 0; j < M; j++)
    {
      m = m + 1.0;
      double x     = xim1 + hi * ( m / MM);
      int    index = (i * M) + j;
      ePos( index) = x;
    }
  }
  return;
}

double EC::get_L2_special( VectorXd e, int i)
{
  int    M  = 3;
  double MM = 3.0;
  double L2 = 0.0;

  int numElems = ms[i]->getNumElems();
  for( int j = 0; j < numElems; j++)
  {
    elem   element = ms[i]->getElem(j);
    double tmp     = 0.0;
    for( int k = 0; k < M; k++)
    {
      int index = (j * M) + k;
      tmp = e( index) * e( index);
    }
    double hj  = element.getLength();
    tmp       *= (hj / MM);
    L2        += tmp;
  }
  L2 = std::sqrt( L2);

  return L2;
}

void EC::calculate_norms()
{
  for( int i = 0; i < es.size(); i++)
  {
    VectorXd e  = es[i];
    VectorXd de = edxs[i];

    double L2 = get_L2_special( e, i);
    L_2.push_back( L2);

    double Linf = e.lpNorm<Eigen::Infinity>();
    L_inf.push_back( Linf);

    double nrg = get_L2_special( de, i);
    NRG.push_back( nrg);
  }
  return;
}

void EC::calculate_orders()
{
  order( L_2, L_2_order);
  order( L_inf, L_inf_order);
  order( NRG, NRG_order);
  return;
}

void EC::order( std::vector<double>& norms, 
                std::vector<double>& order)
{
  for( int i = 0; i < norms.size(); i++)
  {
    double ord = 0.0;
    if ( i == 0)
    {
      ord = 0.0;
    }
    else 
    {
      double top    = std::log( norms[i-1] / norms[i]);
      double bottom = std::log( 2);
      ord           = top / bottom;
    }
    order.push_back( ord);
  }
  return;
}

void EC::write_errors()
{
  std::ofstream file;
  std::string name = "error_results_case_"
                    + static_cast<std::ostringstream*>( 
                      &(std::ostringstream() << cn) )->str()
                    + ".txt";
  file.open( name.c_str());

  // Print e(x) for N+1 = 10, 20, 40 for cases 1 and 2; plot
  if( (cn == 1) || (cn ==2) )
  {
    std::ofstream f;
    std::string   n = "plot_case_"
                      + static_cast<std::ostringstream*>( 
                        &(std::ostringstream() << cn) )->str()
                      + ".txt";
    f.open( n.c_str());
    
    for( int i = 0; i < 3; i++)
    {
      int Np1 =  ms[i]->getNumElems();
      f << "e" << Np1 << "  = " << std::endl;
      f << es[i]     << std::endl;
      f << "x" << Np1 << " = "    << std::endl;
      f << xE[i]     << std::endl;
    }
    f.close();
  }

  // Print norms and orders for all cases for tables
  file << std::scientific;
  vector_printer( file, "L_2", L_2);
  file << std::fixed;
  vector_printer( file, "L_2_order", L_2_order);

  file << std::scientific;
  vector_printer( file, "L_inf", L_inf);
  file << std::fixed;
  vector_printer( file, "L_inf_order", L_inf_order);

  file << std::scientific;
  vector_printer( file, "NRG", NRG);
  file << std::fixed;
  vector_printer( file, "NRG_order", NRG_order);

  file.close();
  return;
}

void EC::vector_printer( std::ofstream&      f, 
                         const std::string&  name, 
                         std::vector<double> v)
{
  f << name << " = " << std::endl;
  for ( int i = 0; i < v.size(); i++)
  {
    f << v[i] << std::endl;
  }
  return;
}
