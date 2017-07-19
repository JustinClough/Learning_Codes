#ifndef A4_UTIL_HPP
#define A4_UTIL_HPP

// ParaFEDDy Headers
#include "A4_FESolver.hpp"

// Trilinos Headers
#include <Teuchos_YamlParameterListHelpers.hpp>

namespace A4{

/// @brief Initializes services like mpi, simmetrix, ect.
///        Instantiates needed classes.
/// @param argc The argument count from main.
/// @param argv The argument variables from main.
/// @param pp   Reference to the parameter list.
void begin(
  int argc,
  char** argv,
  Teuchos::Ptr<Teuchos::ParameterList> &pp);

/// @brief Take one Finite Element - Dislocation Dynamics step.
/// @param argv The argument variables from main.
/// @param pp   Reference to the parameter list.
void take_step(
  char** argv,
  Teuchos::Ptr<Teuchos::ParameterList> &pp);


/// @brief Finalizes services like mpi.
void finish();

} // End namespace A4

#endif
