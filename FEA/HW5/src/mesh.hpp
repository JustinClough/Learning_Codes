#ifndef MESH_HPP
#define MESH_HPP

#include "eig_wrap.hpp"

class mesh
{
  public:
    // Constructor. Creates nodes and elements
    mesh( int Np1_, bool isL_);

    // Print mesh information:
    //  - number of nodes
    //  - number of elements
    //  - location of nodes (node_matrix)
    //  - node collections of elements (elem_matrix)
    //  - nodes on each geometric edge
    void print_mesh_stats();

    // Returns a pointer to a perturbed 
    //  version of this mesh.
    mesh* get_perturbed();

    // Deconstructor
    ~mesh();

  private:
    // True if the mesh is an L shape
    bool isL;

    // The value of N (from N+1).
    //  The number of interior nodes.
    int N;

    // Number of elements
    int num_elems;

    // Number of total mesh nodes
    int num_nodes;

    // The num_nodes by 2 matrix of node locations
    MatrixXd node_matrix;

    // The num_elems by 3 matrix of node locations
    MatrixXd elem_matrix;

    // The num_elems vector of element areas
    std::vector<double> elem_areas;

    // Calcuates the element areas, 
    //  populates elem_areas
    void calc_areas();

    // Calcuates the area of a single element.
    //  Returns the area of element i.
    double calc_elem_area( int i);

    // Creates the nodes of the mesh, 
    //  popuates the node_matrix
    void create_nodes();

    // Creates the elements of the mesh,
    //  populates the elem_matrix;
    void create_elems();

    // Pointer to nodes on bottom of mesh
    std::vector<int> bottom_nodes;

    // Pointer to nodes on right of mesh
    std::vector<int> right_nodes;

    // Pointer to nodes on left of mesh
    std::vector<int> left_nodes;

    // Pointer to nodes on top of mesh
    std::vector<int> top_nodes;

    // Checks where a node is and places it into the 
    //  correct array
    void check_boundary( int index);

    // Creates a single element number i from 
    //  a triple of nodes n1, n2, n3
    void create_elem_from_triple( int i, int n1, int n2, int n3);

    // Perturb the mesh by adjusting nodal locations
    void perturb();
  
    // Perturbs node i of the mesh
    void perturb_node( int i, bool bottom, bool top, bool left, bool right);
};

#endif