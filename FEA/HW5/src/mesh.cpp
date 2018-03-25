#include "mesh.hpp"

#include "eig_wrap.hpp"

#include <iostream>
#include <cstdlib>

mesh::mesh( int Np1_, bool isL_)
{
  isL = isL_;

  // TODO
  if ( isL)
  {
    std::cout
      << "L shaped domains not supported."
      << std::endl;
    std::abort();
  }

  N   = Np1_ - 1;

  num_nodes = (N+2) * (N+2);
  node_matrix = MatrixXd::Zero( num_nodes, 2);

  create_nodes();

  num_elems = 2 * (N+1) * (N+1);

  elem_matrix = MatrixXd::Zero( num_elems, 3);

  create_elems();
}

mesh::~mesh()
{
  bottom_nodes.clear();
  right_nodes.clear();
  left_nodes.clear();
  top_nodes.clear();
}

void mesh::create_nodes()
{
  // x and y spacing between nodes
  double dx = 1.0 / ( (int)N + 1.0);
  double dy = 1.0 / ( (int)N + 1.0);

  double x     = 0.0;
  double y     = 0.0;
  int    index = 0;

  for( int i = 0; i < (N+2); i++)
  {

    for( int j = 0; j < (N+2); j++)
    {
      node_matrix( index, 0) = x;
      node_matrix( index, 1) = y;

      x += dx;

      check_boundary( index);

      index++;
    }
    x =  0.0;
    y += dy;

  }

  return;
}

void mesh::check_boundary( int index)
{
  // Check top or bottom
  if ( index <= (N+1) )
  {
    bottom_nodes.push_back( index);
  }
  else if ( index >= ((N+2)*(N+1) ) )
  {
    top_nodes.push_back( index);
  }

  // Check left or right
  if ( index % (N+2) == 0)
  {
    left_nodes.push_back( index);
  }
  else if ( index % (N+1) == 0 )
  {
    right_nodes.push_back( index);
  }

  return;
}

void mesh::create_elems()
{
  int elem = 0;
  int n1   = 0;
  int n2   = 0;
  int n3   = 0;
  
  for( int n = 0; n < ( (N+2) * (N+1)); n++)
  {
    if ( n % (N+1) != 0 || n == 0)
    {
      n1 = n;
      n2 = n + 1;
      n3 = n + N + 2;
      create_elem_from_triple( elem, n1, n2, n3);
      elem++;

      n1 = n;
      n2 = n + N + 2;
      n3 = n + N + 1;
      create_elem_from_triple( elem, n1, n2, n3);
      elem++;
    }
  }

  return;
}

void mesh::create_elem_from_triple( int i, int n1, int n2, int n3)
{
  elem_matrix( i, 0) = n1;
  elem_matrix( i, 1) = n2;
  elem_matrix( i, 2) = n3;
}

void mesh::print_mesh_stats()
{
  std::cout
    << "The number of nodes = " << num_nodes
    << std::endl;

  std::cout 
    << "The number of elements = " << num_elems
    << std::endl;

  std::cout 
    << "node_matrix = " 
    << std::endl 
    << node_matrix
    << std::endl;

  std::cout 
    << "elem_matrix = " 
    << std::endl 
    << elem_matrix
    << std::endl;

  std::cout 
    << "Nodes on bottom are: "
    << std::endl;
  for( size_t i = 0; i < bottom_nodes.size(); i++)
  {
    std::cout << bottom_nodes[i] << std::endl;
  }

  std::cout 
    << "Nodes on top are: "
    << std::endl;
  for( size_t i = 0; i < top_nodes.size(); i++)
  {
    std::cout << top_nodes[i] << std::endl;
  }

  std::cout 
    << "Nodes on left are: "
    << std::endl;
  for( size_t i = 0; i < left_nodes.size(); i++)
  {
    std::cout << left_nodes[i] << std::endl;
  }

  std::cout 
    << "Nodes on right are: "
    << std::endl;
  for( size_t i = 0; i < right_nodes.size(); i++)
  {
    std::cout << right_nodes[i] << std::endl;
  }

  return;
}
