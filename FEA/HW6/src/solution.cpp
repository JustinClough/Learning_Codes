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
  int n = mesh->get_num_nodes();

  K( 0, 0) = 1.0;
  for( int i = 0; i < K.cols(); i++)
  {
    if( i != 0)
    {
      K( 0, i) = 0.0;
    }
  }
  Force( 0) = 0.0;
  
  K( n-1, n-1) = 1.0;
  for( int i = 0; i < (K.cols() - 1); i++)
  {
    K( n-1, i) = 0.0;
  }
  Force( n -1) = 0.0;

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

  return;
}

void Solution::solve( double T_)
{
  T = T_;
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
  *u = K.fullPivLu().solve( Force);
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

  return;
}

void Solution::backward_euler_solve( double T)
{
  MatrixXd K     = M / dt + S;
  VectorXd Force = F;
  MatrixXd P     = M / dt;

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
  return;
}

void Solution::crank_nicolson_solve( double T)
{
  MatrixXd K     = M  + S * dt / 2.0;
  VectorXd Force = F;
  MatrixXd P     = M  - S * dt / 2.0;

  double time = dt;
  int index   = 0;
  while( time <= T)
  {
    Force = P * ( *(U[index]));

//    std::cout 
//      << "M = "
//      << std::endl
//      << M
//      << std::endl
//      << "S = "
//      << std::endl
//      << S
//      << std::endl
//      << "dt = " << dt << std::endl;
  

    assign_boundary_conditions( K, Force);
    VectorXd* u = linear_solve( K, Force);

//    std::cout 
//      << "Ax = b"
//      << std::endl
//      << "A = "
//      << std::endl
//      << K
//      << std::endl
//      << "b = "
//      << std::endl
//      << Force
//      << std::endl
//      << "x = "
//      << std::endl
//      << *u
//      << std::endl;

//    std::cout << "Exact Solution at time = " << time << std::endl;
//    for( int i = 0; i < mesh->get_num_nodes(); i++)
//    {
//      double position = mesh->get_pos( i);
//      double solution = get_analytical_solution( position, time);
//
//      std::cout 
//        << "u(" << position << ") = "
//        << solution
//        << std::endl;
//    }
//
//    std::cout << "My Solution at time = " << time << std::endl;
//    for( int i = 0; i < mesh->get_num_nodes(); i++)
//    {
//      double position = mesh->get_pos( i);
//
//      std::cout 
//        << "Uh(" << position << ") = "
//        << (*u)(i)
//        << std::endl;
//    }
  
    U.push_back( u);

    time += dt;
    index++;
  }

  return;
}

double Solution::get_analytical_derv( double x, double time)
{
  double answer = 0.0;
  if( CaseNumber == 1)
  {
    answer = 2.0 * std::cos( 2.0 * x) * std::exp( -4.0 * time);
  }
  else if( CaseNumber == 2)
  {
    // TODO
  }

  return answer;
}

double Solution::get_analytical_solution( double x, double time)
{
  double answer = 0.0;
  if( CaseNumber == 1)
  {
    answer = std::sin( 2.0 * x) * std::exp( -4.0 * time);
  }
  else if ( CaseNumber == 2)
  {
    // TODO
  }

  return answer;
}

double Solution::linear_interpolant(
                    double xl,
                    double xr,
                    double x,
                    double fl,
                    double fr)
{
  double slope = (fr - fl) / (xr - xl);

  double ans = fl + (x - xl) * slope; 

  return ans;
}

double Solution::get_elemental_L2( int i)
{
  double M = 3.0;

  VectorXd sol = *( U[ U.size() -1]);
  double time = dt * (U.size() - 1);

//  std::cout << "Exact Solution at time = " << time << std::endl;
//  for( int j = 0; j < mesh->get_num_nodes(); j++)
//  {
//    double position = mesh->get_pos( j);
//    double solution = get_analytical_solution( position, time);
//
//    std::cout 
//      << "u(" << position << ") =   "
//      << solution
//      << std::endl;
//    std::cout 
//      << "sol(" << position << ") = "
//      << sol(j)
//      << std::endl;
//  }
//

  int nl = 0;
  int nr = 0;
  mesh->get_elem( i)->get_indices( &nl, &nr);

  double uhl = sol( nl);
  double uhr = sol( nr);

  double xl = mesh->get_pos( nl);
  double xr = mesh->get_pos( nr);
  double h = mesh->get_elem( i)->get_length();

  double x1 = xl + h / M * 1.0;
  double x2 = xl + h / M * 2.0;
  double x3 = xl + h / M * 3.0;

  double u1 = get_analytical_solution( x1, time);
  double u2 = get_analytical_solution( x2, time);
  double u3 = get_analytical_solution( x3, time);
  
  double uh1 = linear_interpolant( xl, xr, x1, uhl, uhr);
  double uh2 = linear_interpolant( xl, xr, x2, uhl, uhr);
  double uh3 = linear_interpolant( xl, xr, x3, uhl, uhr);

  double e1 = uh1 - u1;
  double e2 = uh2 - u2;
  double e3 = uh3 - u3;

//  std::cout 
//    << "u1 = " << u1
//    << std::endl
//    << "u2 = " << u2
//    << std::endl
//    << "u3 = " << u3
//    << std::endl;
//
//  std::cout 
//    << "uh1 = " << uh1
//    << std::endl
//    << "uh2 = " << uh2
//    << std::endl
//    << "uh3 = " << uh3
//    << std::endl;
//
//  std::cout 
//    << "e1 = " << e1
//    << std::endl
//    << "e2 = " << e2
//    << std::endl
//    << "e3 = " << e3
//    << std::endl;

  double e1sq = e1 * e1;
  double e2sq = e2 * e2;
  double e3sq = e3 * e3;

//  std::cout 
//    << "e1^2 = " << e1sq 
//    << std::endl
//    << "e2^2 = " << e2sq 
//
//    << std::endl
//    << "e3^2 = " << e3sq 
//    << std::endl;

  double ans = e1sq + e2sq + e3sq;

  ans *= ( h / M);


  return ans;
}

void Solution::compute_L2_error()
{
  double error = 0.0;
  for( int i = 0; i < mesh->get_num_elems(); i++)
  {
    double tmp = get_elemental_L2( i);
    error += tmp;
  }
  
  L2_error = std::sqrt( error);
  return;
}

double max_3( double x, double y, double z)
{
  if( x > y && x > z)
  {
    return x;
  }
  else if( y > x && y > z)
  {
    return y;
  }
  else if( z > x && z > y)
  {
    return z;
  }
  else
  {
    std::cout 
      << "Error calculating max( . . .)"
      << std::endl;

    std::abort();
  }

}

double Solution::get_elemental_Linf( int i)
{
  double M = 3.0;

  VectorXd sol = *( U[ U.size() -1]);
  double time = dt * (U.size() - 1);

  int nl = 0;
  int nr = 0;
  mesh->get_elem( i)->get_indices( &nl, &nr);

  double uhl = sol( nl);
  double uhr = sol( nr);

  double xl = mesh->get_pos( nl);
  double xr = mesh->get_pos( nr);
  double h = mesh->get_elem( i)->get_length();

  double x1 = xl + h / M * 1.0;
  double x2 = xl + h / M * 2.0;
  double x3 = xl + h / M * 3.0;

  double u1 = get_analytical_solution( x1, time);
  double u2 = get_analytical_solution( x2, time);
  double u3 = get_analytical_solution( x3, time);
  
  double uh1 = linear_interpolant( xl, xr, x1, uhl, uhr);
  double uh2 = linear_interpolant( xl, xr, x2, uhl, uhr);
  double uh3 = linear_interpolant( xl, xr, x3, uhl, uhr);

  double e1 = std::abs(uh1 - u1);
  double e2 = std::abs(uh2 - u2);
  double e3 = std::abs(uh3 - u3);

  double ans = max_3( e1, e2, e3);

  return ans;
}

void Solution::compute_Linf_error()
{
  double winner = 0.0;
  double challenger = 0.0;
  for( int i = 0; i < mesh->get_num_elems(); i++)
  {
    challenger = get_elemental_Linf( i);

    if( challenger > winner)
    {
      winner = challenger;
    }
  }
  Linf_error = winner;
  return;

}

void Solution::compute_H1_error()
{
  double error = 0.0;
  for( int i = 0; i < mesh->get_num_elems(); i++)
  {
    double tmp = get_elemental_H1( i);
    error += tmp;
  }

  error += L2_error;
  
  H1_error = std::sqrt( error);
  return;
}

double Solution::get_elemental_H1( int i)
{
  double M = 3.0;

  VectorXd sol = *( U[ U.size() -1]);
  double time = dt * (U.size() - 1);

  int nl = 0;
  int nr = 0;
  mesh->get_elem( i)->get_indices( &nl, &nr);

  double uhl = sol( nl);
  double uhr = sol( nr);

  double xl = mesh->get_pos( nl);
  double h = mesh->get_elem( i)->get_length();

  double x1 = xl + h / M * 1.0;
  double x2 = xl + h / M * 2.0;
  double x3 = xl + h / M * 3.0;

  double u1 = get_analytical_derv( x1, time);
  double u2 = get_analytical_derv( x2, time);
  double u3 = get_analytical_derv( x3, time);
  
  double uh = (uhr - uhl) / h;

  double e1 = uh - u1;
  double e2 = uh - u2;
  double e3 = uh - u3;

  double e1sq = e1 * e1;
  double e2sq = e2 * e2;
  double e3sq = e3 * e3;

  double ans = e1sq + e2sq + e3sq;

  ans *= ( h / M);

  return ans;
}

void Solution::calculate_errors()
{
  compute_L2_error();
  compute_H1_error();
  compute_Linf_error();
  return;
}

void Solution::print_data()
{
  for( size_t i = 0; i < U.size(); i++)
  {
    std::cout 
      << "U[" << i << "] = "
      << std::endl
      << *(U[i])
      << std::endl;
  }
  std::cout  << std::scientific
    << std::endl
    << "Case Number: " << CaseNumber
    << std::endl
    << "Method:      ";
    if( meth == fe)
    {
      std::cout << "Forward Euler";
    }
    else if( meth == be)
    {
      std::cout << "Backward Euler";
    }
    else if( meth == cn)
    {
      std::cout << "Crank-Nicolson";
    }
    std::cout
    << std::endl
    << "Total time:  " << T
    << std::endl
    << "Time step:   " << dt
    << std::endl
    << "N+1:         " << mesh->get_num_nodes() - 1
    << std::endl
    << "L2_error:    " << L2_error
    << std::endl
    << "H1_error:    " << H1_error
    << std::endl
    << "Linf_error:  " << Linf_error
    << std::endl
    << std::endl;

  // TODO
  return;
}

void Solution::clear_solutions()
{
  for( size_t i = 0; i < U.size(); i++)
  {
    delete U[i];
  }
  U.clear();

  L2_error = 0.0;
  H1_error = 0.0;
  return;
}

