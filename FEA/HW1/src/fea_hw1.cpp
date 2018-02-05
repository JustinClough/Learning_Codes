#include "eig_wrap.hpp"
#include "mesh1D.hpp"
#include "stiffness.hpp"
#include "forcing.hpp"
#include "errorCalcs.hpp"

#include <iostream>

int main( int argc, char** argv)
{
  // Define Mesh paramters, testing size
  double oddSize         = 0.9;
  double evenSize        = 1.1;
  //int    NplusOneArray[] = {10, 20, 40, 80, 160, 320};
  int    NplusOneArray[] = {10, 10, 10, 10, 10, 10};
  int    numTests        = 6;
  int caseNumber = 1;

  std::vector<VectorXd> solutions;

  for( int i = 0; i < numTests; i++)
  {
    // First construct the mesh 
    int NplusOne = NplusOneArray[i]; 
    mesh1D* mesh = new mesh1D( oddSize, evenSize, NplusOne);

    springFactory* sf = new springFactory( mesh, caseNumber);
    sf->create_stiffness();
    MatrixXd K = sf->getStiffness();

    forcingFactory* ff = new forcingFactory( mesh, caseNumber);
    ff->create_forcing();
    VectorXd F = ff->getForcing();

    solutions.push_back( K.colPivHouseholderQr().solve( F));

    delete ff;
    delete sf;
    delete mesh;
  }
  errorCalcs( solutions, NplusOneArray, caseNumber); 
  solutions.clear();
  

  return 0;
}
