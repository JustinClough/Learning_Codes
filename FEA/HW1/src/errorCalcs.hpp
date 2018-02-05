#ifndef ERRORCALCS_HPP
#define ERRORCALCS_HPP

#include "eig_wrap.hpp"
#include "mesh1D.hpp"

void errorCalcs( std::vector<VectorXd> solutions,
                 std::vector<mesh1D*> meshes, 
                 int* NA, 
                 int caseNumber);

class EC
{
  public:
    // Constructor
    EC( std::vector<VectorXd> solutions,
        std::vector<mesh1D*> meshes,
        int* NA, 
        int caseNumber);

    // Create the analytical solution, dependent on case
    void create_analyticals();

    // Calculates the L2, L_inf, and Energy; includes convergence
    void calc_errors();

    // Writes calculation results to file
    void write();

  private:
    // vector of FE solution vectors
    std::vector<VectorXd> sols;

    // The Array of NplusOne values
    int* na;

    // The Case Number for this problem
    int cn;

    // The vector of analytical solutions
    std::vector<VectorXd> us;

    // The vector of mesh pointers
    std::vector<mesh1D*> ms;

    // Calculate the actual error as a difference
    void getDiff();

    // A vector of error vectors
    std::vector<VectorXd> ehs;

    // Calculates the L2 norm and order
    void L_2();

    // Calculates the L_inf norm and order
    void L_inf();

    // Calculates the Energy norm and order
    void Energy();
};

#endif
