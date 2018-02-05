#ifndef STIFFNESS_HPP
#define STIFFNESS_HPP

#include "mesh1D.hpp"
#include "eig_wrap.hpp"

class springFactory
{
  public:
  // Constructor
  springFactory( mesh1D* mesh, int caseNumber_);

  // Create the stiffness matrix for the problem
  void create_stiffness();

  // Get the stiffness matrix for the problem
  MatrixXd getStiffness();

  private:
  // Assign the row and column value of the stiffness matrix
  // for the particular case number
  void assign_stiffness( int row, int col);

  // Assign stiffness component for p=3, q=2 case (caseNumber 1)
  void p3q2Stiffness( int row, int col);

  // Assign stiffness component for p=x+1, q=0 case (caseNumber 2)
  void pXq0Stiffness( int row, int col);

  // The stiffness matrix
  MatrixXd K;

  // The case number for this problem
  int caseNumber;

  // Pointer to the mesh
  mesh1D* m;

  // Number of dofs
  int numDofs;
};
#endif
