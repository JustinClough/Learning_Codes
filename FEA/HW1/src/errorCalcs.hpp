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

    // The vector of L_2 norms
    std::vector<double> L2;

    // The vector of L_2 orders
    std::vector<double> L_2_order;

    // Calculates the L_inf norm and order
    void L_inf();

    // The vector of L_inf norms
    std::vector<double> Linf;

    // The vector of L_inf orders
    std::vector<double> L_inf_order;

    // Calculates the Energy norm and order
    void Energy();

    // The vector of NRG norms
    std::vector<double> NRG;

    // The vector of NRG orders
    std::vector<double> NRG_order;

    // An intra element interpolation of the error
    double e_interp( double ea, 
                     double eb, 
                     double xa, 
                     double xb, 
                     double h, 
                     double x);
};

#endif
