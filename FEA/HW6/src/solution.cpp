#include "solution.hpp"

#include <iostream>
#include <cstdlib>

double Solution::calc_pi()
{
  return std::atan( 1.0) * 4.0;
}

Solution::Solution( Mesh* mesh_, 
                    int CaseNumber_, 
                    int method_, 
                    double dt_     )
{

  mesh       = mesh_;
  CaseNumber = CaseNumber_;
  meth       = assign_method( method_);
  dt         = dt_;

  pi = calc_pi();

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

void Solution::assemble_background_force()
{
  // since f(x,t) =def= 0 there is no
  // work to be done.
  return;
}


void Solution::assemble_system()
{
  assemble_stiffness();
  assemble_mass();

  assemble_background_force();

  return;
}

void Solution::assign_boundary_conditions( 
                MatrixXd &K, 
                VectorXd &Force)
{
  double left_bc_value  = 0.0;
  double right_bc_value = 0.0;

  int n = mesh->get_num_nodes();

  double diag = K( 0, 0);
  for( int i = 0; i < K.cols(); i++)
  {
    if( i != 0)
    {
      K( 0, i) = 0.0;
    }
  }
  Force( 0) = diag * left_bc_value;
  
  diag = K( n-1, n-1);
  for( int i = 0; i < (K.cols() - 1); i++)
  {
    K( n-1, i) = 0.0;
  }
  Force( n-1) = diag * right_bc_value;
  
  return;
}

double Solution::get_analytic_IC( double x)
{
  double answer = 0.0;
  if( CaseNumber == 1)
  {
    answer = std::sin( 2.0 * x);
  }
  else if( CaseNumber == 2)
  {
    if( x < (pi/2.0) )
    {
      answer = x;
    }
    else
    {
      answer = x - pi;
    }
  }
  return answer;
}

VectorXd Solution::get_u_proj_elem( int i)
{
  VectorXd u_elem = VectorXd::Zero( 2);

  int nL = 0;
  int nR = 0;
  mesh->get_elem( i)->get_indices( &nL, &nR);

  double xL = mesh->get_pos( nL);
  double xR = mesh->get_pos( nR);
  double xM = (xL + xR) / 2.0;

  double ul = get_analytic_IC( xL);
  double um = get_analytic_IC( xM);
  double ur = get_analytic_IC( xR);
  
  u_elem( 0) = ul + 2.0 * um;
  u_elem( 1) = ur + 2.0 * um;

  double h = mesh->get_elem( i)->get_length();

  u_elem *= ( h / 6.0);

  return u_elem;
}

void Solution::set_initial_condition()
{
  VectorXd u_proj = VectorXd::Zero( F.rows());
  for( int i = 0; i < mesh->get_num_elems(); i++)
  {
    VectorXd u_elem = get_u_proj_elem( i);

    u_proj( i)   += u_elem( 0);
    u_proj( i+1) += u_elem( 1);
  }

  // Set Zero DBCs
  u_proj( 0) = 0.0;
  u_proj( u_proj.rows() - 1) = 0.0;

  assign_boundary_conditions( M, u_proj);
  VectorXd* u0 = linear_solve( M, u_proj);
  U.push_back( u0);

  std::cout
    << "u_proj = "
    << std::endl
    << u_proj
    << std::endl;

  std::cout
    << "C_0 = "
    << std::endl
    << *u0
    << std::endl;

  return;
}

void Solution::solve( double T)
{
  set_initial_condition();
  if( meth == fe)
  {
    forward_euler_solve( T);
  }
  else if( meth == be)
  {
    backward_euler_solve( T);
  }
  else if( meth == cn)
  {
    crank_nicolson_solve( T);
  }
  return;
}

VectorXd* Solution::linear_solve( MatrixXd K, VectorXd Force)
{
  VectorXd* u = new VectorXd ();
  *u = (K.fullPivLu().solve( Force));
  return u;
}

void Solution::forward_euler_solve( double T)
{
  MatrixXd K     = M / dt;
  VectorXd Force = F;
  MatrixXd P     = K - S;


  double time = dt;
  int index   = 0;
  while( time < T)
  {
    Force = P * ( *(U[index]));

    assign_boundary_conditions( K, Force);
    VectorXd* u = linear_solve( K, Force);
    U.push_back( u);

    time += dt;
    index++;
  }

  for( size_t index = 0; index < U.size(); index++)
  {
    std::cout
      << "*(U[index]) = *(U[" << index << "]) = "
      << std::endl
      << *(U[index])
      << std::endl;
  }
  
  return;
}

void Solution::backward_euler_solve( double T)
{
  // TODO
  return;
}

void Solution::crank_nicolson_solve( double T)
{
  // TODO
  return;
}

void Solution::calculate_errors()
{
  // TODO
  return;
}

void Solution::print_data()
{
  // TODO
  return;
}

