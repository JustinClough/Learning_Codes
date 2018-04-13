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

  for( size_t i = 0; i < elems.size(); i++)
  {
    delete elems[i];
  }
  elems.clear();

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

  std::cout
    << "Element Lengths: "
    << std::endl;
  double checksum = 0.0;
  double length = 0.0;
  for( size_t i = 0; i < elems.size(); i++)
  {
    length = elems[i]->get_length();
    std::cout
      << "Element " << i << " length: "
      << length
      << std::endl;
    checksum += length;
  }

  std::cout
    << "Total length covered by elements: "
    << checksum
    << std::endl;

  return;
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
  int left  = 0;
  int right = 1;

  double xl  = 0.0;
  double xr  = 0.0;
  for( int i = 0; i < num_elems; i++)
  {
    xl = nodes[ left];
    xr = nodes[ right];

    Element* e = new Element( left, xl, right, xr);
    elems.push_back( e);

    left++;
    right++;
  }

  return;
}

Element* Mesh::get_elem( int i)
{
  return elems[i];
}

