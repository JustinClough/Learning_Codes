#ifndef MESH_HPP
#define MESH_HPP


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

  private:
    // The number of interior mesh points
    // Total mesh points are N+2 to account
    //  for the ends of the mesh.
    int N;

    // The total number of nodes on the mesh.
    int num_nodes;

    // The number of mesh elements
    int num_elems;

};

#endif
