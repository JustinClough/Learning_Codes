#include "mesh.hpp"

#include "eig_wrap.hpp"

#include <iostream>
#include <algorithm>
#include <time.h>
#include <cstdlib>
#include <cmath>


double get_random( double min, double max)
{
  double tmp = (double) rand() / RAND_MAX;
  double ans = min + tmp * ( max - min);

  return ans;
}

mesh::mesh( int Np1_, bool isL_, bool isCurved_)
{
  isL      = isL_;
  isCurved = isCurved_;
  N        = Np1_ - 1;

  L_warning();

  if( isL)
  {
    halfway = (N+2)/2 + 1;

    std::cout << "halfway = " << halfway << std::endl;

    num_nodes = (N+2) * (N+2) - ( (N+1) / 2) * ( (N+1) / 2);
    node_matrix = MatrixXd::Zero( num_nodes, 2);
    create_nodes_L();

    int n_s   = (N+1) / 2;
    int n_l   = N+1;
    num_elems = 2 *(n_l * n_s + n_s * n_s);
    elem_matrix = MatrixXd::Zero( num_elems, 3);
    create_elems_L();

  } 
  else
  {
    num_nodes = (N+2) * (N+2);
    node_matrix = MatrixXd::Zero( num_nodes, 2);
    create_nodes();

    num_elems = 2 * (N+1) * (N+1);
    elem_matrix = MatrixXd::Zero( num_elems, 3);
    create_elems();
  }

  calc_areas();
}

void mesh::create_nodes_L()
{
  // x and y spacing between nodes
  double dx = 2.0 / ( (double)N + 1.0);
  double dy = 2.0 / ( (double)N + 1.0);

  double x     = -1.0;
  double y     = -1.0;
  int    index = 0;

  for( int i = 0; i < (N+2); i++)
  {
    if( i < halfway)
    {
      for( int j = 0; j < (N+2); j++)
      {
        node_matrix( index, 0) = x;
        node_matrix( index, 1) = y;

        x += dx;

        check_boundary( index);

        index++;
      }
    }
    else
    {
      for( int j = 0; j < halfway; j++)
      {
        node_matrix( index, 0) = x;
        node_matrix( index, 1) = y;

        x += dx;

        check_boundary( index);

        index++;
      }
    }
    x =  0.0;
    y += dy;

  }
  return;
}

void mesh::create_elems_L()
{
  int elem = 0;
  int n1   = 0;
  int n2   = 0;
  int n3   = 0;
  
  for( int n = 0; n < ( num_nodes - (halfway +1)); n++)
  {
    std::cout << "num_nodes = " << num_nodes << std::endl;
    std::cout << "n= " << n<< std::endl;

    std::cout << "num_elems = " << num_elems << std::endl;
    std::cout << "elem = " << elem << std::endl;

    if ( n < (N+2) * (halfway -1) )
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
  }
  
  return;
}

void mesh::L_warning()
{
  if( isL)
  {
    if( N % 2 == 0)
    {
      std::cout
        << "Cannot form an L shaped mesh with N = "
        << N
        << std::endl;

      std::cout
        << "N must be an odd number"
        << std::endl;

      std::abort();
    }
  }
  return;
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
  mesh* pm = new mesh( (N+1), isL, isCurved);

  pm->perturb();

  return pm;
}

double mesh::get_elem_side_length( int elem, int side)
{
  return side_lengths( elem, side);
}

double mesh::get_pos( int elem, int i, int xy)
{
  int n = elem_matrix( elem, i);

  return node_matrix( n, xy); 
}

void mesh::calc_elem_sides( int i)
{
  int n1 = elem_matrix( i, 0);
  int n2 = elem_matrix( i, 1);
  int n3 = elem_matrix( i, 2);

  double x1 = node_matrix( n1, 0);
  double y1 = node_matrix( n1, 1);

  double x2 = node_matrix( n2, 0);
  double y2 = node_matrix( n2, 1);

  double x3 = node_matrix( n3, 0);
  double y3 = node_matrix( n3, 1);

  double x12 = x1 - x2;
  double x23 = x2 - x3;
  double x13 = x1 - x3;

  double y12 = y1 - y2;
  double y23 = y2 - y3;
  double y13 = y1 - y3;
  
  double h1 = std::sqrt( x23 * x23 + y23 * y23);
  double h2 = std::sqrt( x13 * x13 + y13 * y13);
  double h3 = std::sqrt( x12 * x12 + y12 * x12);

  side_lengths(i, 0) = h1;
  side_lengths(i, 1) = h2;
  side_lengths(i, 2) = h3;
}

void mesh::calc_side_lengths()
{
  side_lengths = MatrixXd::Zero( num_elems, 3);
  for( int i = 0; i < num_elems; i++)
  {
    calc_elem_sides( i);
  }
  return;
}

int mesh::get_number_nodes()
{
  return num_nodes;
}

double mesh::get_elem_area( int i)
{
  return elem_areas[i];
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

int mesh::which_boundary( int node)
{
  if ( std::find( bottom_nodes.begin(), bottom_nodes.end(), node)
      != bottom_nodes.end() )
  {
    return 1;
  }
  else if ( std::find( top_nodes.begin(), top_nodes.end(), node) 
      != top_nodes.end() )
  {
    return 3;
  }

  // Check if on left or right boundary
  if ( std::find( left_nodes.begin(), left_nodes.end(), node) 
      != left_nodes.end() )
  {
    return 4;
  }
  else if ( std::find( right_nodes.begin(), right_nodes.end(), node) 
      != right_nodes.end() )
  {
    return 2;
  }

  return 0;
}

double mesh::get_pos( int node, int xy)
{
  return node_matrix( node, xy);
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

  double h = 1.0 / ( (double)N + 1.0);

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
    if( top && isCurved)
    {
      ynew = y_beta_curve( xnew);
    }
  }

  node_matrix( i, 0) = xnew;
  node_matrix( i, 1) = ynew;

  return;
}

double mesh::y_beta_curve( double x)
{
  double beta = 1.0 / 4.0;
  return y_beta_curve( x, beta);
}

double mesh::y_beta_curve( double x, double beta)
{
  return beta * x * ( 1.0 - x) + 1.0;
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

  return std::abs(area);
}

void mesh::create_nodes()
{
  if( isCurved)
  {
    double dx = 1.0 / ( (double)N + 1.0);

    double x     = 0.0;
    double y     = 0.0;
    int    index = 0;

    for( int i = 0; i < (N+2); i++)
    {
      for( int j = 0; j < (N+2); j++)
      {
        double Y_top = y_beta_curve( x);
        double dy    = Y_top / ( double (N) +1.0);
        y = ((double) i) * dy;

        node_matrix( index, 0) = x;
        node_matrix( index, 1) = y;

        x += dx;

        check_boundary( index);

        index++;
      }
      x =  0.0;

    }
  }
  else
  {
    // x and y spacing between nodes
    double dx = 1.0 / ( (double)N + 1.0);
    double dy = 1.0 / ( (double)N + 1.0);

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
  }

  return;
}

void mesh::check_boundary( int index)
{
  bool interior = true;

  // TODO: isL condition

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

int mesh::get_global_id( int elem, int node)
{
  return elem_matrix( elem, node);
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
    << std::scientific
    << checksum
    << std::endl;

  return;
}
