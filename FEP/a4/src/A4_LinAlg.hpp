#ifndef A4_LIN_ALG_HPP
#define A4_LIN_ALG_HPP

/// @file A4_LinAlg.hpp

#include "A4_Defines.hpp"
#include "A4_Disc.hpp"

namespace A4 {

/// @brief A general linear algebra object container.
struct LinAlgData
{
  /// @brief The stiffness matrix.
  RCP<Matrix> K;
  /// @brief The solution vector.
  RCP<Vector> U;
  /// @brief The load vector.
  RCP<Vector> F;
};

/// @brief A container for parallel linear algebra data
class LinAlg
{

  public:

    /// @brief Construct the linear algebra object.
    /// @param d The relevant discretization object.
    LinAlg(Disc* disc);

    /// @brief Destroy the linear algebra object.
    /// @details This will destroy the owned and ghost data objects.
    ~LinAlg();

    /// @brief Transfer data from ghost->K to owned->K.
    /// @details This is called with the Tpetra::ADD directive.
    void gather_K();

    /// @brief Transfer data from owned->F to ghost->F.
    /// @details This is called with the Tpetra::ADD directive.
    void gather_F();

    /// @brief The owned linear algebra data.
    LinAlgData* owned;

    /// @brief The ghost linear algebra data.
    LinAlgData* ghost;

  private:

    RCP<Import> importer;
    RCP<Export> exporter;

};

/// @brief Create a linear algebra object.
/// @param d The relevant discretization object.
LinAlg* create_lin_alg(Disc* d);

/// @brief Destroy a linear algebra object.
/// @param l The linear algebra object to destroy.
void destroy_lin_alg(LinAlg* l);

} // end namespace A4

#endif
