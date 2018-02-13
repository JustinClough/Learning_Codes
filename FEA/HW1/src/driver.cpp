#include "driver.hpp"

#include <iostream>

void drive_problem( double oddSize, 
                    double evenSize,
                    int*   NplusOneArray,
                    int    numTests,
                    int    caseNumber)
{
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

    std::cout << "K = " << std::endl;
    std::cout << K << std::endl;

    // Third construct forcing vector
    forcingFactory* ff = new forcingFactory( mesh, caseNumber);
    ff->create_forcing();
    VectorXd F = ff->getForcing();

    std::cout << "F = " << std::endl;
    std::cout << F << std::endl;

    // Calculate and store solution
    solutions.push_back( K.colPivHouseholderQr().solve( F));

    delete ff;
    delete sf;
  }
  for (int i = 0; i < solutions.size(); i++)
  {
    std::cout << "sol[" << i << "] = " << std::endl;
    std::cout << solutions[i] << std::endl;
  }
  
  errorCalcs( solutions, meshes, caseNumber); 
  solutions.clear();
  for (int i = 0; i < meshes.size(); i++)
  {
    delete meshes[ i];
  }
  meshes.clear();

  return;
}
