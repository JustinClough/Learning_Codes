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

    // Assembles the linear system
    void assemble_system();

    // Assigns boundary conditions to the system
    void assign_boundary_conditions();

    // Solves the linear system up to time T
    void solve( double T);

    // Calculates the L2 and H1 solution errors
    void calculate_errors();

    // Prints information to the screen
    void print_data();

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

    // End simulation time
    double T;

    // Global stiffness matrix
    MatrixXd S;

    // Global mass matrix
    MatrixXd M;

    // Global Background Forcing Vector
    VectorXd F;

    // Vector of pointers to solutions
    std::vector< VectorXd* > U;

    // Assembles only the stiffness matrix
    void assemble_stiffness();

    // Assembles only the mass matrix
    void assemble_mass();
};

#endif
