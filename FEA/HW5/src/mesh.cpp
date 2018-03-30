#include "mesh.hpp"

#include "eig_wrap.hpp"

#include <iostream>
#include <algorithm>
#include <time.h>
#include <cstdlib>


double get_random( double min, double max)
{
  double tmp = (double) rand() / RAND_MAX;
  double ans = min + tmp * ( max - min);

  return ans;
}

mesh::mesh( int Np1_, bool isL_)
{
  isL = isL_;

  // TODO: bonus part 1
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
  calc_areas();
}

mesh::~mesh()
{
  bottom_nodes.clear();
  right_nodes.clear();
  left_nodes.clear();
  top_nodes.clear();
  
  elem_areas.clear();
}

mesh* mesh::get_perturbed()
{
  mesh* pm = new mesh( (N+1), isL);

  pm->perturb();

  return pm;
}

int mesh::get_number_nodes()
{
  return num_nodes;
}

int mesh::get_number_elements()
{
  return num_elems;
}

void mesh::perturb()
{
  for( int i = 0; i < num_nodes; i++)
  {
    bool bottom = false;
    bool top    = false;
    bool left   = false;
    bool right  = false;

    // Check if on bottom or top boundary
    if ( std::find( bottom_nodes.begin(), bottom_nodes.end(), i) 
          != bottom_nodes.end() )
    {
      bottom = true;
    }
    else if ( std::find( top_nodes.begin(), top_nodes.end(), i) 
               != top_nodes.end() )
    {
      top = true;
    }

    // Check if on left or right boundary
    if ( std::find( left_nodes.begin(), left_nodes.end(), i) 
          != left_nodes.end() )
    {
      left = true;
    }
    else if ( std::find( right_nodes.begin(), right_nodes.end(), i) 
               != right_nodes.end() )
    {
      right = true;
    }

    perturb_node( i, bottom, top, left, right);
  }

  // Now correct area information
  elem_areas.clear();
  calc_areas();

  return;
}

void mesh::perturb_node( int i, 
                         bool bottom, 
                         bool top, 
                         bool left, 
                         bool right)
{
  double max = (1.0 / 2.0) / 10.0;
  double min = -max;
  double r = get_random( min, max);

  double h = 1.0 / ( (int)N + 1.0);

  double p = r * h;
  double x = node_matrix( i, 0);
  double y = node_matrix( i, 1);

  double xnew = x;
  double ynew = y;

  if( !bottom && !top)
  {
    ynew += p;
  }

  if( !left && !right)
  {
    xnew += p;
  }

  node_matrix( i, 0) = xnew;
  node_matrix( i, 1) = ynew;

  return;
}

void mesh::calc_areas()
{
  for( int i = 0; i < num_elems; i++)
  {
    elem_areas.push_back( calc_elem_area( i));
  }

  return;
}

double mesh::calc_elem_area( int i)
{
  int n1 = elem_matrix( i, 0);
  int n2 = elem_matrix( i, 1);
  int n3 = elem_matrix( i, 2);

  double x1 = node_matrix( n1, 0);
  double x2 = node_matrix( n2, 0);
  double x3 = node_matrix( n3, 0);

  double y1 = node_matrix( n1, 1);
  double y2 = node_matrix( n2, 1);
  double y3 = node_matrix( n3, 1);

  double v1x = x2 - x1;
  double v1y = y2 - y1;

  double v2x = x3 - x1;
  double v2y = y3 - y1;

  double cross = (v1x * v2y) - (v1y * v2x);

  double area = ( 1.0 / 2.0) * cross;

  return area;
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
  bool interior = true;


  // Check top or bottom
  if ( index <= (N+1) )
  {
    bottom_nodes.push_back( index);
    interior = false;
  }
  else if ( index >= ((N+2)*(N+1) ) )
  {
    top_nodes.push_back( index);
    interior = false;
  }

  // Check left or right
  if ( index % (N+2) == 0)
  {
    left_nodes.push_back( index);
    interior = false;
  }
  else if ( index % (N+2) == (N+1) )
  {
    right_nodes.push_back( index);
    interior = false;
  }

  if( interior)
  {
    interior_nodes.push_back( index);
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
    if ( n % (N+2) != (N+1) || n == 0)
    {
      n1 = n;
      n2 = n + 1;
      n3 = n + N + 3;
      create_elem_from_triple( elem, n1, n2, n3);
      elem++;

      n1 = n;
      n2 = n + N + 3;
      n3 = n + N + 2;
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

int mesh::get_number_interior_nodes()
{
  return interior_nodes.size();
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

  std::cout
    << "Area of elements are: "
    << std::endl;
  double checksum = 0.0;
  for( size_t i = 0; i < elem_areas.size(); i++)
  {
    double area = elem_areas[i];
    std::cout << area << std::endl;
    checksum += area;
  }

  std::cout
    << "Total area covered by the elements: " 
    << checksum
    << std::endl;

  return;
}
