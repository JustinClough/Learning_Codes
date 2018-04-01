#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include "mesh.hpp"

class solution
{
  public:
    // Constructor
    solution( mesh* m_, int CaseNumber_);

    // Assembles the stiffness and forcing functions
    void assemble_problem();

    // Apply the Dirichlet boundary conditions
    //  for the specified case number.
    void apply_boundary_conditions();

    // Solves the system KU=F for U
    void solve_system();

    // Compute the errors
    void compute_errors();

    // Destructor
    ~solution();

  private:
    // Pointer to the mesh
    mesh* m;

    // PDE Parameter "p"
    double p;

    // PDE Parameter "q"
    double q;

    // Number of mesh nodes
    int numNodes;

    // Number of elements in the mesh
    int numElems;

    // Number of degrees of freedom per element
    int elemental_dofs;

    // CaseNumber
    int CaseNumber;

    // Global Reduced Stiffness Matrix
    SparseMatrix<double> K;

    // Global Reduced Forcing Vector
    VectorXd F;

    // Solution Vector
    VectorXd U;

    // Assembles only the stiffness matrix
    void assemble_stiffness();

    // Assembles only the forcing matrix
    void assemble_forcing();

    // Returns the stiffness matrix for element i
    MatrixXd get_elemental_stiffness( int i);

    // Assigns the i'th elemental stiffness k_elem
    //  to the global stiffness matrix
    void assign_elemental_stiffness( MatrixXd k_elem, int i);

    // Gets the elemental pure mass matrix
    MatrixXd get_elemental_M( int i);

    // Gets the elemental pure spring matrix
    MatrixXd get_elemental_S( int i);

    // Gets the elemental forcing function.
    //  Body forces only.
    VectorXd get_elemental_forcing( int elem_num);

    // Assigns the elemental forcing vector to the global one.
    void assign_elemental_forcing( VectorXd f_elem, int elem);

    // Gets the force at a point based on the 
    //  case nuber defined at construction time.
    double force_at_point( double x, double y);

    // Adjusts the global system for boundary value bv
    //  at node i.
    void fix_global_system( double bv, int i);

    // Gets the DBC boundary value for node number i
    //  based on Case number defined at construction.
    double get_boundary_value( int i);
};

#endif
