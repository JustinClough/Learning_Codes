#include "mesh.hpp"

#include <cmath>
#include <iostream>

double calc_pi()
{
  return std::atan( 1.0) * 4.0;
}

Mesh::Mesh( int Np1_)
{
  N = Np1_ - 1;
  num_nodes = N + 2;
  num_elems = num_nodes - 1;

  pi = calc_pi();

  create_nodes();
  create_elems();

  return;
}

Mesh::~Mesh()
{
  nodes.clear();

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

void Mesh::print_stats()
{
  std::cout
    << "The number of nodes = " << num_nodes
    << std::endl;

  std::cout 
    << "The number of elements = " << num_elems
    << std::endl;

  std::cout 
    << "Nodal Locations :"
    << std::endl;
  for( size_t i = 0; i < nodes.size(); i++)
  {
    std::cout
      << "Node " << i << ": "
      << nodes[i]
      << std::endl;
  }

}

void Mesh::create_nodes()
{
  const double x0 = 0.0;
  const double L  = pi;

  const double dx = (L - x0) / num_elems;

  double x = 0.0;
  for( int i = 0; i < num_nodes; i++)
  {
    nodes.push_back( x);
    x += dx;
  }

  return;
}

void Mesh::create_elems()
{
  // TODO
  return;
}


