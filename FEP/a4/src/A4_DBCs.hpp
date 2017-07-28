#ifndef A4_DBCS_HPP
#define A4_DBCS_HPP

/// @file A4_DBCs.hpp

#include "A4_Disc.hpp"

namespace Teuchos {
class ParameterList;
}

namespace A4 {

using Teuchos::ParameterList;

class LinAlg;

/// @brief Prescribe Dirichlet boundary conditions.
/// @param d  The discretization of node sets.
/// @param la The relevant linear algebra data.
/// @param p  The parameterlist defining bcs.
void apply_dbcs(
    Disc* d,
    LinAlg* la,
    ParameterList const& p);

}

#endif
