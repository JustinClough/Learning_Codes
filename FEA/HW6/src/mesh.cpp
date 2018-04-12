#include "mesh.hpp"

Mesh::Mesh( int Np1_)
{
  N = Np1_ - 1;
  num_nodes = N + 2;
  num_elems = num_nodes - 1;

  return;
}

Mesh::~Mesh()
{

}

int Mesh::get_num_nodes()
{
  return num_nodes;
}

int Mesh::get_num_elems()
{
  return num_elems;
}

int Mesh::get_num_interior_nodes()
{
  return N;
}
