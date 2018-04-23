#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include "mesh.hpp"
#include "printer.hpp"

class Solution
{
  public:
    // Constructs the solution.
    Solution( Mesh* mesh_, Printer* printer_, int method_);
   
    // Destructs the solution 
    ~Solution();

    // Assembles the linear system
    void assemble_system();

    // Solves the linear system
    void solve();

    // Calculates the L2 and H1 solution errors
    void calculate_errors();

    // Prints information to the screen
    void print_data();

  private:
    // Method to earth mesh
    int method;

    // Pointer to the mesh
    Mesh* mesh;

    // Pointer to the printer
    Printer* printer;

    // Global stiffness matrix
    MatrixXd K;

    // Global Background Forcing Vector
    VectorXd F;

    // Solution Vector
    VectorXd U;

    // The L2 norm error
    double L2_error;
  
    // The H1 norm error
    double H1_error;

    // The L_inf norm error
    double Linf_error;

    // Estimate value of pi
    double pi;

    // Assembles only the stiffness matrix
    void assemble_stiffness();

    // Assembles only the force vector
    void assemble_force();

    // Solve the passed linear system
    // Ax = b for x
    void linear_solve( );

    // Assigns boundary conditions to the system
    void assign_boundary_conditions();

    // Computes the L2 error
    void compute_L2_error();

    // Computes the H1 error
    void compute_H1_error();

    // Computes the L_inf error
    void compute_Linf_error();

    // Gets the analytical solution
    double get_analytical_solution( double x);

    // Gets the analytical solution space derivative
    double get_analytical_derv( double x);

    // Gets the elemental L2 error for element i
    double get_elemental_L2( int i);

    // Returns the linear interpolant 
    double linear_interpolant( 
                    double xl,
                    double xr,
                    double x,
                    double fl,
                    double fr  );

    // Gets the elemental H1 error
    double get_elemental_H1( int i);

    // Gets the elemental Linf error
    double get_elemental_Linf( int i);

    // Returns the analytical force at x
    double get_analytical_force( double x);
};

#endif
