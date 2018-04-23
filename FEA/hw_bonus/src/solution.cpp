#include "solution.hpp"

#include <iostream>
#include <cstdlib>

double calc_pi()
{
  return 4.0 * atan( 1.0)
}

Solution::Solution( Mesh*    mesh_, 
                    Printer* printer_,
                    int      method_  )
{

  mesh    = mesh_;
  printer = printer_;
  method  = method_;

  pi = calc_pi();

  int num_nodes = mesh->get_num_nodes();
  
  K = MatrixXd::Zero( num_nodes, num_nodes);
  F = VectorXd::Zero( num_nodes);
  U = VectorXd::Zero( num_nodes);
}

Solution::~Solution()
{
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

    K( L, L) += k_elem( 0, 0);
    K( L, R) += k_elem( 0, 1);
    K( R, L) += k_elem( 1, 0);
    K( R, R) += k_elem( 1, 1);
  }

  return;
}

void Solution::assemble_force()
{
  // TODO
  return;
}


void Solution::assemble_system()
{
  assemble_stiffness();
  assemble_force();

  return;
}

void Solution::assign_boundary_conditions()
{
  int n = mesh->get_num_nodes();

  // TODO
  double diag = K( 0, 0);
  for( int i = 0; i < K.cols(); i++)
  {
    if( i != 0)
    {
      K( 0, i) = 0.0;
    }
  }
  F( 0) = 0.0;
  
  K( n-1, n-1) = 1.0;
  for( int i = 0; i < (K.cols() - 1); i++)
  {
    K( n-1, i) = 0.0;
  }
  F( n -1) = 0.0;

  return;
}

void Solution::solve()
{
  assign_boundary_conditions();
  linear_solve();
  return;
}

void Solution::linear_solve()
{
  U = K.fullPivLu().solve( F);
  return;
}

double Solution::get_analytical_derv( double x)
{
  double tmp1 = pi * std::cos( pi * x)
  double tmp2 = std::sin( pi * x);
  double expx = std::exp( x);

  double answer = (tmp1 + tmp2) * expx;

  return answer;
}

double Solution::get_analytical_solution( double x)
{
  // TODO
  (void) x;
  double answer = 0.0;

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

  VectorXd sol = U;

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

  double u1 = get_analytical_solution( x1);
  double u2 = get_analytical_solution( x2);
  double u3 = get_analytical_solution( x3);
  
  double uh1 = linear_interpolant( xl, xr, x1, uhl, uhr);
  double uh2 = linear_interpolant( xl, xr, x2, uhl, uhr);
  double uh3 = linear_interpolant( xl, xr, x3, uhl, uhr);

  double e1 = uh1 - u1;
  double e2 = uh2 - u2;
  double e3 = uh3 - u3;

  double e1sq = e1 * e1;
  double e2sq = e2 * e2;
  double e3sq = e3 * e3;

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
    if( x == y)
    {
      return x;
    }
    else if( x == z)
    {
      return x;
    }
    else
    {
      return y;
    } 
  }

}

double Solution::get_elemental_Linf( int i)
{
  double M = 3.0;

  VectorXd sol = U;

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

  double u1 = get_analytical_solution( x1);
  double u2 = get_analytical_solution( x2);
  double u3 = get_analytical_solution( x3);
  
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

  error += L2_error * L2_error;
  
  H1_error = std::sqrt( error);
  return;
}

double Solution::get_elemental_H1( int i)
{
  double M = 3.0;

  VectorXd sol = U;

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

  double u1 = get_analytical_derv( x1);
  double u2 = get_analytical_derv( x2);
  double u3 = get_analytical_derv( x3);
  
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
  int num_nodes = mesh->get_num_nodes() - 1;
  printer->add( 
                num_nodes,  
                L2_error,   
                Linf_error,   
                H1_error   );

  return;
}
