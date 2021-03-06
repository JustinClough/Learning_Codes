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

    // Writes the errors to a text file
    void write_errors();

    // Calculates the norms of the error vectors
    void calculate_norms();

    // Calculate the order of convergence
    void calculate_orders();

  private:
    // The vector of FEM solutions
    std::vector<VectorXd> sols;

    // The vector of FEM meshes
    std::vector<mesh1D*> ms;

    // The case number
    int cn;

    // The vector of error vectors
    std::vector<VectorXd> es;

    // The vector of derivative error vectors
    std::vector<VectorXd> edxs;

    // The vector of error positions
    std::vector<VectorXd> xE;

    // Populate the error position vector
    void error_positions( VectorXd& ePos, mesh1D* mesh, int M);

    // Populate the error value vector
    void eval_errors( VectorXd& e, 
                      VectorXd& ePos, 
                      VectorXd& sol, 
                      mesh1D*   mesh,
                      int       M);

    // Populate the derivative error vector
    void eval_ders( VectorXd& e, 
                    VectorXd& dedx, 
                    VectorXd& ePos, 
                    VectorXd& sol, 
                    mesh1D*   mesh,
                    int       M);

    // Get the analytical solution at a give position
    double get_analytic( double x);

    // Get the derivative of the analytical solution at
    //  a given position
    double get_analytic_der( double x);

    // Get the interpolation of the FEM solution
    double get_interp( double ua, 
                       double ub,  
                       elem element, 
                       double x);

    // The vector of L_2 norms
    std::vector<double> L_2;

    // The vector of L_inf norms
    std::vector<double> L_inf;

    // The vector of NRG norms
    std::vector<double> NRG;

    void order( std::vector<double>& norms, 
                std::vector<double>& order);

    // The vector of L_2_order
    std::vector<double> L_2_order;

    // The vector of L_inf_order
    std::vector<double> L_inf_order;

    // The vector of NRG_order
    std::vector<double> NRG_order;

    // Prints a vector to file
    void vector_printer( std::ofstream&      f, 
                         const std::string&  name, 
                         std::vector<double> v);

    // Gets the L2 error as specified in the handout
    double get_L2_special( VectorXd e, int i);
};

#endif
