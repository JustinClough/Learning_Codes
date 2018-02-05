#ifndef FORCING_HPP
#define FORCING_HPP

#include "mesh1D.hpp"
#include "eig_wrap.hpp"

class forcingFactory
{
  public:
  // Constructor
  forcingFactory( mesh1D* mesh, int caseNumber_);

  // Create the forcing vector for the problem
  void create_forcing();

  // Get the forcing vector for the problem
  VectorXd getForcing();

  private:
  // Assign the row value of the forcing vector
  // for the particular case number
  void assign_force( int row);

  // Assign forcing component for p=3, q=2 case (caseNumber 1)
  void p3q2force( int row);

  // Assign forcing component for p=x+1, q=0 case (caseNumber 2)
  void pXq0force( int row);

  // Evaluate given force function for p3q2 case
  double analytic_p3q2( double x);

  // Evaluate given force function for pXq0 case
  double analytic_pXq0( double x);

  // The forcing vector
  VectorXd F;

  // The case number for this problem
  int caseNumber;

  // Pointer to the mesh
  mesh1D* m;

  // Number of dofs
  int numDofs;

  // Force assignment for case 3
  void case3force( int row);

  // Analytical force for case 3
  double analytic_3( double x);
  // Analytical force for case 4
  double analytic_4( double x);
  // Analytical force for case 5
  double analytic_5( double x);

  // Force assignment for case 4
  void case4force( int row);

  // Force assignment for case 5
  void case5force( int row);
};
#endif
