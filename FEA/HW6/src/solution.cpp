#include "solution.hpp"

#include <iostream>
#include <cstdlib>

Solution::Solution( Mesh* mesh_, 
                    int CaseNumber_, 
                    int method_, 
                    double dt_     )
{

  mesh       = mesh_;
  CaseNumber = CaseNumber_;
  meth       = assign_method( method_);
  dt         = dt_;

  int num_nodes = mesh->get_num_nodes();
  
  S = MatrixXd::Zero( num_nodes, num_nodes);
  M = MatrixXd::Zero( num_nodes, num_nodes);
  F = VectorXd::Zero( num_nodes);

}

Solution::~Solution()
{
  for( size_t i = 0; i < U.size(); i++)
  {
    delete U[i];
  }
  U.clear();
}

Method Solution::assign_method( int method_)
{
  Method method;
  if( method_ == 1)
  {
    method = fe;
  }
  else if( method_ == 2)
  {
    method = be;
  }
  else if( method_ == 3)
  {
    method = cn;
  }
  else
  {
    std::cout
      << "Error assigning method"
      << std::endl
      << "Attempted assignment: "
      << method_
      << std::endl;

    std::abort();
  }

  return method;
}

void Solution::assemble_stiffness()
{
  MatrixXd k_elem;
  int L = 0;
  int R = 0;
  for( int i = 0; i < mesh->get_num_elems(); i++)
  {
    Element* elem = mesh->get_elem(i);

    k_elem = elem->get_stiffness();
    elem->get_indices( &L, &R);

    S( L, L) += k_elem( 0, 0);
    S( L, R) += k_elem( 0, 1);
    S( R, L) += k_elem( 1, 0);
    S( R, R) += k_elem( 1, 1);
  }

  return;
}

void Solution::assemble_mass()
{
  MatrixXd m_elem;
  int L = 0;
  int R = 0;
  for( int i = 0; i < mesh->get_num_elems(); i++)
  {
    Element* elem = mesh->get_elem(i);

    m_elem = elem->get_mass();
    elem->get_indices( &L, &R);

    M( L, L) += m_elem( 0, 0);
    M( L, R) += m_elem( 0, 1);
    M( R, L) += m_elem( 1, 0);
    M( R, R) += m_elem( 1, 1);
  }
  return;
}


void Solution::assemble_system()
{
  assemble_stiffness();

  std::cout << "S = "
    << std::endl 
    << S
    << std::endl ;

  assemble_mass();

  std::cout << "M = "
    << std::endl 
    << M
    << std::endl ;

  // TODO
  return;
}

void Solution:: assign_boundary_conditions()
{
  // TODO
  return;
}

void Solution:: solve( double T)
{
  // TODO
  return;
}

void Solution:: calculate_errors()
{
  // TODO
  return;
}

void Solution:: print_data()
{
  // TODO
  return;
}

