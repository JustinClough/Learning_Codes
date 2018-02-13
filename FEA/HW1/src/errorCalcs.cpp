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
    int numDofs   = ms[i]->getNumNodes() - 1;
    VectorXd e    = VectorXd::Zero( M * numDofs)
    VectorXd ePos = VectorXd::Zero( M * numDofs)


    es.push_back( e);
    xE.push_back( ePos);
  }
  return;
}
