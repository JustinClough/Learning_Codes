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
                 int* NA, 
                 int caseNumber)
{
  EC* ec = new EC( solutions, meshes, NA, caseNumber);

  ec->create_analyticals();
  ec->calc_errors();
  ec->write();
  
  delete ec;
  return;
}

void EC::create_analyticals()
{
  for( int i = 0; i < ms.size(); i++)
  {
    int numDofs = na[i] - 1;
    VectorXd U = VectorXd::Zero( numDofs);
    // For each mesh, we need to calculate the exact nodal 
    //  solutions to use a the analytic solution
    for( int j = 0; j < numDofs; j++)
    {
      double xj = ms[i]->getElem(j).getRightPos();
      if ( cn == 1)
      { 
        // This is the p3q2 case
        U( j) = xj * (xj - 1.0) * ( std::sin( 5.0 * xj) + 3.0 * std::exp( xj));
      }
      else if ( cn == 2)
      {
        // This is the p(x), q0 case
        // TODO
    
      }
    }
    us.push_back( U);
  }
  return;
}

void EC::calc_errors()
{
  getDiff();
  L_2();
  L_inf();
  Energy();
  return;
}

void EC::getDiff()
{
  for( int i = 0; i < us.size(); i++)
  {
    ehs.push_back( sols[i] - us[i] );
  }
  return;
}

double EC::e_interp( double ea, 
                 double eb, 
                 double xa, 
                 double xb, 
                 double h, 
                 double x)
{
  return ea * (xb - x) / h + eb * (x - xa) / h;
}

void EC::L_2()
{
  double tmp = 0.0;
  for( int i = 0; i < ehs.size(); i++)
  {
    VectorXd e = ehs[i];
    mesh1D* m = ms[i];

    for( int j = 0; j < e.size(); j++)
    {
      double hj = m->getElem( j).getLength();
      double xa = m->getElem( j).getLeftPos();
      double xb = m->getElem( j).getRightPos();
      double ea = 0.0;
      if( j != 0)
      {
        double ea = e(j-1);
      }
      double eb = e(j);
      // M perscribed in problem handout
      int M = 3;
      for ( int m = 1; m <= M; m++)
      {
        double x = xa + hj * m / M;
        double e_intp = e_interp( ea, eb, xa, xb, hj, x);
        tmp = e_intp * e_intp;
      }
      tmp *=  hj / M;
    }
    L2.push_back( std::sqrt( tmp));
  }

  std::cout << "L2 Error = " << std::endl;
  for ( int i =0; i < L2.size(); i++)
  {
    std::cout << L2[i] << std::endl;
  }

  for ( int i = 0; i < L2.size(); i++)
  {
    if ( i == 0)
    {
      L_2_order.push_back( 0.0);
    }
    else
    {
      double top    = std::log( L2[ i-1] / L2[i]);
      double bottom = std::log( 2.0);
      double tmp    = top / bottom;
      L_2_order.push_back( tmp);
    }
  }
  return;
}

void EC::L_inf()
{
  double tmp = 0.0;
  for( int i = 0; i < ehs.size(); i++)
  {
    VectorXd e = ehs[i];
    mesh1D* m = ms[i];

    for( int j = 0; j < e.size(); j++)
    {
      double hj = m->getElem( j).getLength();
      double xa = m->getElem( j).getLeftPos();
      double xb = m->getElem( j).getRightPos();
      double ea = 0.0;
      if( j != 0)
      {
        double ea = e(j-1);
      }
      double eb = e(j);
      // M perscribed in problem handout
      int M = 3;
      for ( int m = 1; m <= M; m++)
      {
        double x = xa + hj * m / M;
        double e_intp = std::abs( e_interp( ea, eb, xa, xb, hj, x) );
        if ( tmp < e_intp)
        {
          tmp = e_intp;
        }
      }
    }
    Linf.push_back( tmp);
    tmp = 0.0;
  }

  std::cout << "Linf Error = " << std::endl;
  for ( int i =0; i < Linf.size(); i++)
  {
    std::cout << Linf[i] << std::endl;
  }

  for ( int i = 0; i < Linf.size(); i++)
  {
    if ( i == 0)
    {
      L_inf_order.push_back( 0.0);
    }
    else
    {
      double top    = std::log( Linf[ i-1] / Linf[i]);
      double bottom = std::log( 2.0);
      double tmp    = top / bottom;
      L_inf_order.push_back( tmp);
    }
  }
  return;
}

void EC::Energy()
{
  double tmp = 0.0;
  for( int i = 0; i < ehs.size(); i++)
  {
    VectorXd e = ehs[i];
    mesh1D* m = ms[i];

    for( int j = 0; j < e.size(); j++)
    {
      double hj = m->getElem( j).getLength();
      double xa = m->getElem( j).getLeftPos();
      double xb = m->getElem( j).getRightPos();
      double ea = 0.0;
      if( j != 0)
      {
        double ea = e(j-1);
      }
      double eb = e(j);
      // M perscribed in problem handout
      int M = 3;
      for ( int m = 1; m <= M; m++)
      {
        // Since we have a linear basis, the 
        //  derivatives are constant inside each element
        double dedx = (eb - ea) / hj;
        tmp = dedx * dedx;
      }
      tmp *=  hj / M;
    }
    NRG.push_back( std::sqrt( tmp));
  }

  std::cout << "NRG Error = " << std::endl;
  for ( int i =0; i < NRG.size(); i++)
  {
    std::cout << NRG[i] << std::endl;
  }

  for ( int i = 0; i < NRG.size(); i++)
  {
    if ( i == 0)
    {
      NRG_order.push_back( 0.0);
    }
    else
    {
      double top    = std::log( NRG[ i-1] / NRG[i]);
      double bottom = std::log( 2.0);
      double tmp    = top / bottom;
      NRG_order.push_back( tmp);
    }
  }
  return;
}

void EC::write()
{
  std::string filename = "errorCalcs_";
  if( cn == 1)
  {
    filename = filename + "1.txt";
  }
  std::ofstream file;
  file.open( filename.c_str());
  
  for( int i = 0; i < ehs.size(); i++)
  {
    file << "Error with " << na[i] << " elements:" << std::endl;
    file << ehs[i] << std::endl;
    file << "Abs(Error) with " << na[i] << " elements:" << std::endl;
    file << ehs[i].cwiseAbs() << std::endl;
    file << "Node Locations: " << std::endl;
    for( int j = 0; j < (ms[i]->getNumElems() - 1); j++)
    {
      file << ms[i]->getElem(j).getRightPos() << std::endl;
    }

    file << "L2 Error: " << std::endl;
    for( int j = 0; j < L2.size(); j++)
    {
      file << L2[j] << std::endl;
    }
    file << "L2 Order: " << std::endl;
    for( int j = 0; j < L_2_order.size(); j++)
    {
      file << L_2_order[j] << std::endl;
    }

    file << "Linf Error: " << std::endl;
    for( int j = 0; j < Linf.size(); j++)
    {
      file << Linf[j] << std::endl;
    }
    file << "Linf Order: " << std::endl;
    for( int j = 0; j < L_inf_order.size(); j++)
    {
      file << L_inf_order[j] << std::endl;
    }

    file << "Energy Error: " << std::endl;
    for( int j = 0; j < NRG.size(); j++)
    {
      file << NRG[j] << std::endl;
    }
    file << "NRG Order: " << std::endl;
    for( int j = 0; j < NRG_order.size(); j++)
    {
      file << NRG_order[j] << std::endl;
    }
  }

  file.close();

  return;
}

EC::EC( std::vector<VectorXd> solutions,
        std::vector<mesh1D*> meshes,
        int* NA, 
        int caseNumber)
{
  sols = solutions;
  ms   = meshes;
  na   = NA;
  cn   = caseNumber;
}
