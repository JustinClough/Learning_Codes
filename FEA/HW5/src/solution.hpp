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

    // Destructor
    ~solution();

  private:
    // Pointer to the mesh
    mesh* m;

    // Number of interior nodes
    int numIntNodes;

    int numElems;

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

};

#endif
