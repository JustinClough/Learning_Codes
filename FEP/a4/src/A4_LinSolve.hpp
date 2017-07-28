#ifndef A4_LIN_SOLVE_HPP
#define A4_LIN_SOLVE_HPP

/// @file A4_LinSolve.hpp

namespace Teuchos {
class ParameterList;
}

namespace A4 {

using Teuchos::ParameterList;

/// @brief Solve a linear system iteratively.
/// @param p A parameter list.
/// @param la The linear algebra data.
/// @param disc The relevant discretization object.
void solve_linear_system(
    ParameterList const& p,
    LinAlg* la,
    Disc* disc);

}

#endif
