#include "eig_wrap.hpp"
#include "mesh1D.hpp"
#include "stiffness.hpp"

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

  for( int i = 0; i < numTests; i++)
  {
    // First construct the mesh 
    int NplusOne = NplusOneArray[i]; 
    mesh1D* mesh = new mesh1D( oddSize, evenSize, NplusOne);

    springFactory* sf = new springFactory( mesh, caseNumber);

    sf->create_stiffness();

    MatrixXd K = sf->getStiffness();

    std::cout << "K = " << std::endl;
    std::cout << K << std::endl;

    delete sf;
    delete mesh;
  }

  return 0;
}
