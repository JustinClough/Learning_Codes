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


    // Solves the linear system up to time T
    void solve( double T);

    // Calculates the L2 and H1 solution errors
    void calculate_errors();

    // Prints information to the screen
    void print_data();

    // Clears the old solutions
    void clear_solutions();

  private:
    // Estimated value of pi = 3.14...
    double pi;

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
  
    // Solve with forward Euler scheme up to
    //  final time T.
    void forward_euler_solve( double T);

    // Solve with backward Euler scheme up to
    //  final time T.
    void backward_euler_solve( double T);

    // Solve with Crank Nicolson scheme up to
    //  final time T.
    void crank_nicolson_solve( double T);

    // Solve the passed linear system
    VectorXd* linear_solve( MatrixXd K, VectorXd Force);

    // Sets the initial conditions
    void set_initial_condition();

    // Gets the analytical solution
    double get_analytic_IC( double x);

    // Estimates the value of pi
    double calc_pi();

    // Gets the projected initial condition for element i
    VectorXd get_u_proj_elem( int i);

    // Assembles the background forcing vector
    void assemble_background_force();

    // Assigns boundary conditions to the system
    void assign_boundary_conditions( 
                  MatrixXd &K, 
                  VectorXd &Force  );

    // The L2 norm error
    double L2_error;
  
    // The H1 norm error
    double H1_error;

    // Computes the L2 error
    void compute_L2_error();

    // Computes the H1 error
    void compute_H1_error();

};

#endif
