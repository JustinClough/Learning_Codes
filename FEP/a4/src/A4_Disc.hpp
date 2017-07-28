#ifndef A4_DISC_HPP
#define A4_DISC_HPP

/// @file A4_Disc.hpp

#include <apfNumbering.h>
#include "A4_Defines.hpp"

/// @cond
// forward declarations
namespace apf {
struct Node;
struct StkModels;
class Mesh;
class MeshEntity;
}
/// @endcond

namespace A4 {

/// @brief A discretization container used to:
/// - construct Tpetra maps and graphs that are used to build
///   linear algebra objects.
/// - gather mesh entities associated with Dirichlet and Neumann
///   boundary conditions, as defined in an input model associations
///   file.
class Disc {

  public:

    /// @brief Construct the discretization object.
    /// @param m The input mesh object.
    /// @param assoc The input model association file name.
    Disc(apf::Mesh* m, const char* assoc);

    /// @brief Destroy the discretization object.
    /// @details This will destroy the association objects, and the Tpetra
    /// linear algebra maps and graphs that this object creates.
    ~Disc();

    /// @brief Returns the underlying AA4 mesh.
    apf::Mesh* get_apf_mesh() { return mesh; }

    /// @brief Returns the model association definitions.
    apf::StkModels* get_model_sets() { return sets; }

    /// @brief Returns the owned DOF map.
    RCP<const Map> get_owned_map() { return owned_map; }

    /// @brief Returns the ghost DOF map.
    RCP<const Map> get_ghost_map() { return ghost_map; }

    /// @brief Returns the owned element connectivity graph.
    RCP<const Graph> get_owned_graph() { return owned_graph; }

    /// @brief Returns the ghost element connectivity graph.
    RCP<const Graph> get_ghost_graph() { return ghost_graph; }

    /// @brief Returns the nodal coordinates.
    RCP<MultiVector> get_coords() { return coords; }

    /// @brief Returns the total number of entity DOFs.
    int get_num_dofs(apf::MeshEntity* e);

    apf::Numbering* get_owned_numbering() {return owned_nmbr;}

    /// @brief Returns the local row IDs of DOFs on a mesh entity.
    void get_ghost_lids(apf::MeshEntity* e, std::vector<LO>& lids);

    /// @brief Returns the local row ID of a node
    LO get_owned_lid(apf::Node const& n, int eq);

    /// @brief Returns the number of side sets.
    int get_num_side_sets() const;

    /// @brief Returns the number of node sets.
    int get_num_node_sets() const;

    /// @brief Returns the name of the ith side set.
    std::string get_side_set_name(const int i) const;

    /// @brief Returns the name of the ith node set.
    std::string get_node_set_name(const int i) const;

    /// @brief Returns the sides in a side set.
    /// @param set The name of the side set of interest.
    std::vector<apf::MeshEntity*> const& get_sides(std::string const& set);

    /// @brief Returns the nodes in a node set.
    /// @param set The name of the node set of interest.
    std::vector<apf::Node> const& get_nodes(std::string const& set);

  private:

    void compute_owned_map();
    void compute_ghost_map();
    void compute_graphs();
    void compute_coords();
    void compute_side_sets();
    void compute_node_sets();

    int num_eqs;
    int num_dims;

    apf::Mesh* mesh;
    apf::StkModels* sets;
    apf::FieldShape* shape;
    apf::Numbering* owned_nmbr;
    apf::Numbering* ghost_nmbr;
    apf::GlobalNumbering* global_nmbr;
    apf::DynamicArray<apf::Node> owned;

    RCP<const Comm> comm;
    RCP<const Map> owned_map;
    RCP<const Map> ghost_map;
    RCP<const Map> node_map;
    RCP<MultiVector> coords;
    RCP<Graph> owned_graph;
    RCP<Graph> ghost_graph;

    std::map<std::string, std::vector<apf::Node> > node_sets;
    std::map<std::string, std::vector<apf::MeshEntity*> > side_sets;
};

} // end namespace A4

#endif
