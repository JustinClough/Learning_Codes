#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "element.hpp"

class Mesh
{
  public:
    // Constructor, creates nodes and elements
    Mesh( int Np1_);

    // Destructor, destroys the Mesh object
    ~Mesh();

    // Returns the total number of nodes
    int get_num_nodes();

    // Returns the number of interior nodes
    int get_num_interior_nodes();

    // Returns the number of elements
    int get_num_elems();

    // Print out information about the mesh.
    void print_stats();

    // Gets the ith element
    Element* get_elem( int i);

  private:
    // The number of interior mesh points
    // Total mesh points are N+2 to account
    //  for the ends of the mesh.
    int N;

    // The total number of nodes on the mesh.
    int num_nodes;

    // The number of mesh elements
    int num_elems;

    // Creates the nodes of the mesh.
    // Calculates the locations of the nodes
    void create_nodes();

    // Creates the elements of the mesh.
    void create_elems();

    // Vector of all node locations
    std::vector< double> nodes;

    // Estimated value of pi = 3.14...
    double pi;

    // Vector of all elements
    std::vector< Element*> elems;

};

#endif
