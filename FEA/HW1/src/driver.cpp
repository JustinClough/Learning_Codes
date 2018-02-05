#include "driver.hpp"

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

  return;
}
