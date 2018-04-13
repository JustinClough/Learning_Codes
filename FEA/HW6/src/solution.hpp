#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include "mesh.hpp"

enum Method
{
  fe, // forward_euler
  be, // backward_euler
  cn  // crank_nicolson
};

class Solution
{
  public:
    // Constructs the solution.
    Solution( Mesh* mesh_, int CaseNumber_, int method_, double dt_);
   
    // Destructs the solution 
    ~Solution();

  private:
    // Time integration method
    Method meth;

    // The case number
    int CaseNumber;

    // The time step size
    double dt;

    // Pointer to the mesh
    Mesh* mesh;

    // Assigns the correct method
    Method assign_method( int method_);
};

#endif
