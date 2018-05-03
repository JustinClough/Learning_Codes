#include "solution.hpp"

#include <iostream>
#include <cstdlib>

Solution::Solution( Mesh*    mesh_, 
                    Printer* printer_,
                    int      method_  )
{

  mesh    = mesh_;
  printer = printer_;
  method  = method_;

  pi = 4.0 * atan( 1.0);

  int num_nodes = mesh->get_num_nodes();
  
  K = MatrixXd::Zero( num_nodes, num_nodes);
  F = VectorXd::Zero( num_nodes);
  U = VectorXd::Zero( num_nodes);

  zK = MatrixXd::Zero( num_nodes + 1, num_nodes + 1);
  zF = VectorXd::Zero( num_nodes + 1);
  zU = VectorXd::Zero( num_nodes + 1);
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

double Solution::get_analytical_force( double x)
{
  double tmp1 =  pi * pi * std::sin( pi * x);
  double tmp2 = 2.0 * pi * std::cos( pi * x);
  double tmp3 = std::sin( pi * x);
  double expx = std::exp( x);

  double ans = (tmp1 - tmp2 - tmp3) * expx;

  return ans;
}

VectorXd Solution::get_elem_force( Element* elem)
{
  VectorXd f_elem = VectorXd::Zero( 2);

  int nL = 0;
  int nR = 0;
  elem->get_indices( &nL, &nR);

  double xL = mesh->get_pos( nL);
  double xR = mesh->get_pos( nR);
  double xM = (xL + xR) / 2.0;

  double fl = get_analytical_force( xL);
  double fm = get_analytical_force( xM);
  double fr = get_analytical_force( xR);

  f_elem( 0) = fl + 2.0 * fm;
  f_elem( 1) = fr + 2.0 * fm;

  double h = elem->get_length();

  f_elem *= (h / 6.0);

  return f_elem;
}

void Solution::assemble_force()
{
  VectorXd f_elem;
  int L = 0;
  int R = 0;
  for( int i = 0; i < mesh->get_num_elems(); i++)
  {
    Element* elem = mesh->get_elem(i);

    f_elem = get_elem_force( elem);
    elem->get_indices( &L, &R);

    F( L) += f_elem( 0);
    F( R) += f_elem( 1);
  }

  return;
}


void Solution::assemble_system()
{
  timespec ts;
  clock_gettime( CLOCK_REALTIME, &ts);

  assemble_stiffness();
  assemble_force();

  timespec tf;
  clock_gettime( CLOCK_REALTIME, &tf);

  double b = 1.0e9;
  t_a = b * (tf.tv_sec - ts.tv_sec) + tf.tv_nsec - ts.tv_nsec;
  t_a /= b;

  return;
}

void Solution::assign_boundary_conditions()
{
  assign_nbc();
  if( method == 0)
  {
    assign_left_dbc();
  }
  else if( method == 1)
  {
    assign_zero_average();
  }

  return;
}

void Solution::assign_nbc()
{
  double alpha =  pi;
  double beta  = -pi * std::exp( 1);

  int n = F.rows();

  F( 0)   -= alpha;
  F( n-1) += beta;

  return;
}

void Solution::assign_left_dbc()
{
  double diag      = K( 0, 0);
  double DBC_value = -(1.0 + std::exp( 1)) * pi / ( 1.0 + pi * pi);

  int n = K.cols();
  for( int i = 0; i < n; i++)
  {
    if( i != 0)
    {
      K( 0, i) = 0.0;
    }
  }

  F( 0) = diag * DBC_value;

  return;
}

VectorXd Solution::get_H_vector()
{
  VectorXd H = VectorXd::Zero( K.rows());

  for( int i = 0; i < mesh->get_num_elems(); i++)
  {
    double hi = mesh->get_elem( i)->get_length();

    double inc = hi / 2.0;

    H( i)   += inc;
    H( i+1) += inc;
  }

  return H;
}

void Solution::assign_zero_average()
{
  // TODO

  //     __     __
  //zk = | K | H |
  //     |_H'| g_|

  double corner = 0.0;
  VectorXd H = get_H_vector();
  for( int row = 0; row < zK.rows(); row++)
  {
    for( int col = 0; col < zK.cols(); col++)
    {
      // Upper left block
      if( row < K.rows() && col < K.cols())
      {
        zK( row, col) = K( row, col);
      }
    }
  }

  const int COL = zK.cols();
  for( int row = 0; row < H.rows(); row++)
  {
    zK( row, COL - 1) = H( row);
  }

  const int ROW = zK.rows();
  for( int col = 0; col < H.rows(); col++)
  {
    zK( ROW - 1, col) = H(col);
    corner += H( col);

  }
  
  corner *= 1.0;

  zK( zK.rows() - 1, zK.cols() - 1) = corner;

  for( int row = 0; row < zF.rows(); row++)
  {
    if( row < F.rows())
    {
      zF( row) = F( row);
    }
    else
    {
      zF( row) = 0.0;
    }
  }

  return;
}

void Solution::solve()
{
  timespec ts;
  clock_gettime( CLOCK_REALTIME, &ts);

  assign_boundary_conditions();
  linear_solve();
  
  timespec tf;
  clock_gettime( CLOCK_REALTIME, &tf);

  double b = 1.0e9;
  t_s = b * (tf.tv_sec - ts.tv_sec) + tf.tv_nsec - ts.tv_nsec;
  t_s /= b;

  if( method == 0)
  {
    R = F - K * U;
  }
  else
  {
    for( int i = 0; i < U.rows(); i++)
    {
      U(i) = zU(i);
    }
    zR = zF - zK * zU;
  }

  return;
}

void Solution::linear_solve()
{
  if( method == 0)
  {
    U = K.fullPivLu().solve( F);
  }
  else
  {
    zU = zK.fullPivLu().solve( zF);
  }

  return;
}

double Solution::get_analytical_derv( double x)
{
  double tmp1 = pi * std::cos( pi * x);
  double tmp2 = std::sin( pi * x);
  double expx = std::exp( x);

  double answer = (tmp1 + tmp2) * expx;

  return answer;
}

double Solution::get_analytical_solution( double x)
{
  double front = std::sin( pi * x) * std::exp( x);
  double backC = (1.0 + std::exp( 1)) * pi / ( 1.0 + pi * pi);

  double answer = front - backC;

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
                H1_error,
                t_a,
                t_s        );

  return;
}
