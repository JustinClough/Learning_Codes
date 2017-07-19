#ifndef A4_ELASTIC_STIFFNESS_HPP
#define A4_ELASTIC_STIFFNESS_HPP

/// @file A4_Elastic_Stiffness.hpp

#include <apf.h>
#include <apfDynamicMatrix.h>

namespace A4 {

/// @brief Class to compute element stiffness matrix for linear elasticity.
class ElasticStiffness : public apf::Integrator
{

  public:

    /// @brief Construct the elastic stiffness integrator.
    /// @param m The relevant apf::Mesh structure.
    /// @param o The numerical integration order of accuracy.
    /// @param E Elastic modulus.
    /// @param nu Poisson's ratio.
    ElasticStiffness(
        apf::Mesh* m,
        int o,
        double E,
        double nu);

    /// @brief Set up data as each new element is encountered.
    /// @param me The incoming mesh element.
    void inElement(apf::MeshElement* me);

    /// @brief Work to be done at a single integration point.
    /// @param p The parametric integration point coordinate.
    /// @param w The numerical integration point weight.
    /// @param dv The differential volume (det J) at the integration point.
    /// @details Calling
    /// - this->process(apf::Mesh* m) or
    /// - this->process(apf::MeshElement* e)
    /// will provide this method with the appropriate input parameters.
    void atPoint(apf::Vector3 const& p, double w, double dv);

    /// @brief Finalize data as we leave each element.
    void outElement();

    /// @brief The element stiffness matrix.
    apf::DynamicMatrix Ke;

  private:

    int num_dims;
    int num_elem_nodes;
    int num_elem_dofs;
    
    apf::DynamicMatrix D;
    apf::DynamicMatrix B;
    apf::DynamicMatrix DB;
    apf::DynamicMatrix BT;
    apf::DynamicMatrix K_tmp;

    apf::NewArray<apf::Vector3> dN;

    apf::Mesh* mesh;
    apf::FieldShape* shape;
    apf::MeshElement* mesh_element;
};

} // end namespace A4

#endif
