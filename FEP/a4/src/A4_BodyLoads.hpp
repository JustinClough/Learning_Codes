#ifndef A4_BODYLOADS_HPP
#define A4_BODYLOADS_HPP

/// @file A4_BodyLoads.hpp

#include <A4_Disc.hpp>
#include <A4_LinAlg.hpp>

#include <apf.h>
#include <apfShape.h>
#include <apfDynamicMatrix.h>

namespace Teuchos
{
class ParameterList;
} // End namespace Teuchos

namespace A4
{

class LinAlg;

/// @brief Precribe body loads.
/// @param d  The discretization of node sets.
/// @param la The relavent linear algebra data.
/// @param order
///           The order of numerical integration accuracy.
/// @param g  The value of the body load.
void apply_body_loads(
  Disc* d,
  LinAlg* la,
  int order,
  double* g);

/// @brief Class to compute elemental force vector due to body load.
class BodyLoad :
  public apf::Integrator
{
  public:

    /// @brief Construct elemental body force integrator
    /// @param mesh  The mesh to work with.
    /// @param order The order of numerical integration.
    /// @param load  The body load vector.
    BodyLoad(
        apf::Mesh* mesh,
        int inte_order,
        apf::Vector3 load);


    /// @brief Prepares each new element.
    /// elem The new element to be processed.
    void inElement(
        apf::MeshElement* me);

    /// @brief Work done at each integration point.
    /// @param p  The parametric integration point coordinates.
    /// @param w  The integration point weight.
    /// @param dv The differential volume at the integration point.A
    void atPoint(
        apf::Vector3 const& p,
        double w,
        double dv);

    /// @brief Free and finalize data once done with the element.
    void outElement();

    /// @brief Get the elemental forcing vector due only to body loads.
    apf::DynamicVector get_fe()
    {
      return fe;
    }

  private:
    int order;
    apf::Mesh* m;
    apf::FieldShape* shape;
    apf::MeshElement* elem;

    apf::Vector3 g;
    apf::NewArray<double> N;
    apf::DynamicVector fe;

    int num_dims;
    int num_elem_nodes;
    int num_elem_dofs;

};

} // end namspace A4
#endif
