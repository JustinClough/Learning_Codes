#ifndef A4_FE_SOLVER_HPP
#define A4_FE_SOLVER_HPP

/// @file A4_FESolver.hpp

#include <Teuchos_ParameterList.hpp>
#include "A4_Disc.hpp"
#include "A4_LinAlg.hpp"
#include "A4_PostProc.hpp"

namespace A4 {

using Teuchos::ParameterList;

/// @cond
class Disc;
class ElasticStiffness;
/// @endcond

/// @brief A class to solve a linear elastic FEM problem.
/// @details With modified RHS data to account for dislocation effects.
class FESolver
{

  public:

    /// @brief Construct the solver object.
    /// @param d The relevant discretization object.
    /// @param p The valid FEM solve parameters.
    /// @param order The integration order.
    /// @param load  The body load (x,y,z).
    FESolver(Disc* d, ParameterList const& p, int order, double load[3]);

    /// @brief Destroy the FEM object.
    ~FESolver();

    /// @brief Assemble and solve the linear FEM problem.
    void solve();

    /// @brief Assign the contents of the solution to the field.
    /// @param f The field to write displacements to.
    void set_disp_to_field( apf::Field* f);

    /// @brief Assign the contents of the forcing vector to the field.
    /// @param f The field to write tractions to.
    void set_force_to_field( apf::Field* f);

    /// @brief Cacluate the Cauchy stress and assign to field.
    /// @param f The field to write Cauchy stress to.
    void set_stress_to_field( apf::Field* f);

    /// @brief Calculates the error in displacement
    ///        approximation with an L2 norm.
    double get_error();

  private:

    void assemble_LHS();
    void assemble_RHS();

    Disc* disc;
    ParameterList params;
    LinAlg la;
    int int_order;

    double g[3];

    ElasticStiffness* LHS;
};

} // end namespace A4

#endif
