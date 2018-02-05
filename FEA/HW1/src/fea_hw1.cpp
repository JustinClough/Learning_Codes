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
  int    caseNumber = 1;

  std::vector<VectorXd> solutions;
  std::vector<mesh1D*> meshes;

  for( int i = 0; i < numTests; i++)
  {
    // First construct the mesh 
    int NplusOne = NplusOneArray[i]; 
    mesh1D* mesh = new mesh1D( oddSize, evenSize, NplusOne);
    meshes.push_back( mesh);

    // Second construct stiffness matrix
    springFactory* sf = new springFactory( mesh, caseNumber);
    sf->create_stiffness();
    MatrixXd K = sf->getStiffness();

    // Third construct forcing vector
    forcingFactory* ff = new forcingFactory( mesh, caseNumber);
    ff->create_forcing();
    VectorXd F = ff->getForcing();

    // Calculate and store solution
    solutions.push_back( K.colPivHouseholderQr().solve( F));

    delete ff;
    delete sf;
  }
  errorCalcs( solutions, meshes, NplusOneArray, caseNumber); 
  solutions.clear();
  for (int i = 0; i < meshes.size(); i++)
  {
    delete meshes[ i];
  }
  meshes.clear();

  return 0;
}
