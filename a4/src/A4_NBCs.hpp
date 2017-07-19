#ifndef A4_NBCS_HPP
#define A4_NBCS_HPP

/// @file A4_NBCs.hpp

#include "A4_Disc.hpp"
#include "apfDynamicVector.h"

namespace Teuchos {
class ParameterList;
}

namespace A4 {

using Teuchos::ParameterList;
class LinAlg;

/// @brief Precribe Neumann boundary conditions.
/// @param d  The discretization of node sets.
/// @param la The relavent linear algebra data.
/// @param p  The parameterlist defining bcs. 
void apply_nbcs(
  Disc* d,
  LinAlg* la,
  ParameterList const& p);

class elemTrac : 
  public apf::Integrator
{
  public:

    /// @brief Construct the traction stiffness integrator.
    /// @param d     The discretization object. 
    /// @param la    The linear algebra object.
    /// @param order Order of numerical integration accuracy.
    /// @param val   The component value of traction.
    /// @param eqn   The equation number to modify.
    elemTrac(
      Disc* d,
      int order,
      double val,
      int eqNumber);

    /// @brief Prepare each new element. 
    /// @param elem The mesh element.
    void inElement( apf::MeshElement* element);

    /// @brief Operate at an integration point.
    /// @param p  The parametric coordinates of the integration point
    /// @param w  The integration point weight.
    /// @param dv The differential volume at the integration point.
    /// @deatils Calling
    ///  - this->process(apf::Mesh* m) or
    ///  - this->process(apf::MeshElement* elem)
    /// will provide the input parameters.
    void atPoint( 
      apf::Vector3 const& p, 
      double w,
      double dv);

    /// @brief Finalize data once done with the element.
    void outElement();

    /// @brief Get the elemental forcing vector.
    apf::DynamicVector get_fe()
      { return fe; }

  private:

    apf::DynamicVector fe;

    int num_elem_nodes;
    int num_elem_dofs;
    int num_dims;
    int eqn;
    double value;

    apf::Mesh* mesh;
    apf::FieldShape* shape;
    apf::MeshElement* elem;
};

}

#endif
