#ifndef ERRORCALCS_HPP
#define ERRORCALCS_HPP

#include "eig_wrap.hpp"
#include "mesh1D.hpp"

void errorCalcs( std::vector<VectorXd>, int* NA, int caseNumber);

class EC
{
  public:
    // Constructor
    EC( std::vector<VectorXd> solutions, int* NA, int caseNumber);

    // Create the analytical solution, dependent on case
    void create_analytic();

    // Calculates the L2, L_inf, and Energy; includes convergence
    void calc_errors();

    // Writes calculation results to file
    void write();

  private:
    // FE solution vector of vectors
    std::vector<VectorXd> sols;

    // The Array of NplusOne values
    int* na;

    // The Case Number for this problem
    int cn;

};

#endif
