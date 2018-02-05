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

void EC::L_2()
{
  // TODO
  return;
}

void EC::L_inf()
{
  // TODO
  return;
}

void EC::Energy()
{
  //TODO
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
