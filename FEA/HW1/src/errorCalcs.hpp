#ifndef ERRORCALCS_HPP
#define ERRORCALCS_HPP

#include "eig_wrap.hpp"
#include "mesh1D.hpp"

void errorCalcs( std::vector<VectorXd> solutions,
                 std::vector<mesh1D*> meshes, 
                 int caseNumber);

class EC
{
  public:
    // Constructor
    EC( std::vector<VectorXd> solutions,
        std::vector<mesh1D*> meshes,
        int caseNumber);

    // Calculates the error vectors for all meshes of this case
    void calculate_errors();

  private:
    // The vector of FEM solutions
    std::vector<VectorXd> sols;

    // The vector of FEM meshes
    std::vector<mesh1D*> ms;

    // The case number
    int cn;

    // The vector of error vectors
    std::vector<VectorXd> es;

    // The vector of error positions
    std::vector<VectorXd> xE;
};

#endif
