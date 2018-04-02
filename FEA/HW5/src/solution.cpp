#include "solution.hpp"

#include <iostream>
#include <ctime>

solution::solution( mesh* m_, int CaseNumber_)
{
  m  = m_;
  CaseNumber = CaseNumber_;

  p = 3.0;
  q = 2.0;

  elemental_dofs = 3;

  numNodes = m->get_number_nodes();
  numElems = m->get_number_elements();

  K = SparseMatrix<double, Eigen::RowMajor>(numNodes, numNodes);
  F = VectorXd::Zero( numNodes);
  U = VectorXd::Zero( numNodes);
}

solution::~solution()
{

}

void solution::assemble_problem()
{
  clock_t start_time = std::clock();

  assemble_stiffness();
  assemble_forcing();

  std::cout << std::scientific
    << "Assembly time: "
    << double ( clock() - start_time) / (double) CLOCKS_PER_SEC
    << " seconds."
    << std::endl;

  return;
}

MatrixXd solution::get_elemental_M( int i)
{
  MatrixXd mass = MatrixXd::Zero( elemental_dofs, elemental_dofs);
  double area = m->get_elem_area( i);

  // Diagonal of the 'mass' matrix
  mass( 0, 0) = 2.0;
  mass( 1, 1) = 2.0;
  mass( 2, 2) = 2.0;

  // Off-diagonal entries of 'mass' matrix
  //  upper tri
  mass( 0, 1) = 1.0;
  mass( 0, 2) = 1.0;
  mass( 1, 2) = 1.0;
  //  lower tri
  mass( 1, 0) = 1.0;
  mass( 2, 0) = 1.0;
  mass( 2, 1) = 1.0;

  mass *= ( area / 12.0);

  return mass;
}

MatrixXd solution::get_elemental_S( int i)
{
  MatrixXd spring = MatrixXd::Zero( elemental_dofs, elemental_dofs);
  double area = m->get_elem_area( i);

  double x1 = m->get_pos( i, 0, 0);
  double y1 = m->get_pos( i, 0, 1);

  double x2 = m->get_pos( i, 1, 0);
  double y2 = m->get_pos( i, 1, 1);

  double x3 = m->get_pos( i, 2, 0);
  double y3 = m->get_pos( i, 2, 1);

  double x31 = x3 - x1;
  double x12 = x1 - x2;
  double x23 = x2 - x3;

  double y31 = y3 - y1;
  double y12 = y1 - y2;
  double y23 = y2 - y3;

  // Diagonal Values of spring matrix
  spring( 0, 0) = y23 * y23 + x23 * x23;
  spring( 1, 1) = y31 * y31 + x31 * x31;
  spring( 2, 2) = y12 * y12 + x12 * x12;

  // Upper
  spring( 0, 1) = y23 * y31 + x23 * x31;
  spring( 0, 2) = y23 * y12 + x23 * x12;
  spring( 1, 2) = y31 * y12 + x31 * x12;

  // Lower
  spring( 1, 0) = y23 * y31 + x23 * x31;
  spring( 2, 0) = y23 * y12 + x23 * x12;
  spring( 2, 1) = y31 * y12 + x31 * x12;

  spring /= (4.0 * area);

  return spring;
}


MatrixXd solution::get_elemental_stiffness( int i)
{
  MatrixXd mass = get_elemental_M( i);
  MatrixXd spring = get_elemental_S( i);

  return p * spring + q * mass;
}

void solution::assign_elemental_stiffness( MatrixXd k_elem, int i)
{
  for( int j = 0; j < elemental_dofs; j++)
  {
    for( int n = 0; n < elemental_dofs; n++)
    {
      int Erow = j;
      int Ecol = n;

      int row = m->get_global_id( i, j);
      int col = m->get_global_id( i, n);

      K.coeffRef( row, col) += k_elem( Erow, Ecol);
    }
  }
  return;
}

void solution::assemble_stiffness()
{
  for( int i = 0; i < numElems; i++)
  {
    MatrixXd k_elem = get_elemental_stiffness( i);
    assign_elemental_stiffness( k_elem, i);
  }

  return;
}

void solution::apply_boundary_conditions()
{

  for( int i = 0; i < numNodes; i++)
  {
    if( m->which_boundary( i) != 0)
    {
      double bv  = get_boundary_value( i);
      fix_global_system( bv, i);
    }
  }
  // two bugs make a feature
  K = K.transpose();
  return;
}

double solution::get_boundary_value( int i)
{
  double x = m->get_pos( i, 0);
  double y = m->get_pos( i, 1);

  double ans = 0.0;

  if( CaseNumber == 1)
  {
    ans = 1.0;
  }
  else if( CaseNumber == 2)
  {
    ans = x;
  }
  else if( CaseNumber == 3)
  {
    ans = y;
  }
  else if( CaseNumber == 4)
  {
    double a = y * y * y;
    double b = std::sin( 5.0 * (x + y));
    double c = 2.0 * std::exp( x);
    ans = a + b + c;
  }
  else
  {
    std::cout
    << "Unrecognized CaseNumber. CaseNumber = "
    << CaseNumber 
    << std::endl;

    std::abort();
  }

  return ans;
}

void solution::fix_global_system( double bv, int i)
{
  // Reassign K(i,:) to all zeros except diag
  double diag = 0.0;
  for( SparseMatrix< double>::InnerIterator 
        it( K, i); it; ++it)
  {
    if( it.row() == it.col() )
    {
      diag = it.value();
    }
    else
    {
      it.valueRef() = 0.0;
    }
  }

  F( i) = bv * diag;

  return;
}

void solution::assemble_forcing()
{
  for( int i = 0; i < numElems; i++)
  {
    VectorXd f_elem = get_elemental_forcing( i);
    assign_elemental_forcing( f_elem, i);
  }

  return;
}

double average( double a, double b)
{
  double ans = (a + b) / 2.0;
  return ans;
}

double average( double a, double b, double c)
{
  double ans = ( a + b + c) / 3.0;
  return ans;
}

VectorXd solution::get_elemental_forcing( int elem_num)
{
  VectorXd f_elem = VectorXd::Zero( elemental_dofs);

  double area = m->get_elem_area( elem_num);

  double x1 = m->get_pos( elem_num, 0, 0);
  double y1 = m->get_pos( elem_num, 0, 1);

  double x2 = m->get_pos( elem_num, 1, 0);
  double y2 = m->get_pos( elem_num, 1, 1);

  double x3 = m->get_pos( elem_num, 2, 0);
  double y3 = m->get_pos( elem_num, 2, 1);

  double x12 = average( x1, x2);
  double x13 = average( x1, x3);
  double x23 = average( x2, x3);

  double y12 = average( y1, y2);
  double y13 = average( y1, y3);
  double y23 = average( y2, y3);

  double x123 = average( x1, x2, x3);
  double y123 = average( y1, y2, y3);

  double f1 = force_at_point( x1, y1);
  double f2 = force_at_point( x2, y2);
  double f3 = force_at_point( x3, y3);

  double f12 = force_at_point( x12, y12);
  double f13 = force_at_point( x13, y13);
  double f23 = force_at_point( x23, y23);

  double f123 = force_at_point( x123, y123);

  double tmp1 = 0.0;
  double tmp2 = 0.0;
  double tmp3 = 0.0;

  tmp1 += 3.0 * f1;
  tmp2 += 3.0 * f2;
  tmp3 += 3.0 * f3;

  tmp1 += 4.0 * ( f12 + f13);
  tmp2 += 4.0 * ( f12 + f23);
  tmp3 += 4.0 * ( f13 + f23);

  tmp1 += 9.0 * f123;
  tmp2 += 9.0 * f123;
  tmp3 += 9.0 * f123;

  f_elem(0) = tmp1;
  f_elem(1) = tmp2;
  f_elem(2) = tmp3;

  f_elem *= area / 60.0;

  return f_elem;
}

void solution::assign_elemental_forcing( VectorXd f_elem, int elem)
{
  for( int i = 0; i < elemental_dofs; i++)
  {
    int row  = m->get_global_id( elem, i);
    F( row) += f_elem( i);
  }
  return;
}

double solution::force_at_point( double x, double y)
{
  double ans = 0.0;
  if( CaseNumber == 1)
  {
    ans = 2.0;
  }
  else if( CaseNumber == 2)
  {
    ans = 2.0 * x;
  }
  else if( CaseNumber == 3)
  {
    ans = 2.0 * y;
  }
  else if( CaseNumber == 4)
  {
    double a =  2.0 * y * y * y;
    double b = -18.0 * y;
    double c =  152.0 * std::sin( 5.0 * ( x + y));
    double d = -2.0 * std::exp( x);

    ans = a + b + c + d;
  }
  else
  {
    std::cout
    << "Unrecognized CaseNumber. CaseNumber = "
    << CaseNumber 
    << std::endl;

    std::abort();
  }

  return ans;
}

void solution::solve_system()
{
  K.makeCompressed();
  Eigen::SparseLU< SparseMatrix< double> > solver;

  solver.analyzePattern( K);

  solver.factorize( K);


  clock_t start_time = std::clock();
  U = solver.solve( F);

  std::cout << std::scientific
    << "Solve time: "
    << double ( clock() - start_time) / (double) CLOCKS_PER_SEC
    << " seconds."
    << std::endl;

  return;

}

void solution::compute_errors()
{
  compute_L2_error();
  compute_H1_error();

  return;
}

void solution::compute_L2_error()
{
  double error = 0.0;
  for( int elem = 0; elem < numElems; elem++)
  {
    error += get_elemental_error_L2( elem);
  }

  L2_error = error;
  L2_error = std::sqrt( L2_error);

  error = std::sqrt( error);
  std::cout << std::scientific
    << "L2 Error = "
    << L2_error
    << std::endl;

  return;
}

double solution::get_elemental_error_L2( int elem)
{
  double area = m->get_elem_area( elem);

  double x1 = m->get_pos( elem, 0, 0);
  double y1 = m->get_pos( elem, 0, 1);

  double x2 = m->get_pos( elem, 1, 0);
  double y2 = m->get_pos( elem, 1, 1);

  double x3 = m->get_pos( elem, 2, 0);
  double y3 = m->get_pos( elem, 2, 1);

  double x12 = average( x1, x2);
  double x13 = average( x1, x3);
  double x23 = average( x2, x3);

  double y12 = average( y1, y2);
  double y13 = average( y1, y3);
  double y23 = average( y2, y3);

  double x123 = average( x1, x2, x3);
  double y123 = average( y1, y2, y3);

  double u1 = get_exact_solution( x1, y1);
  double u2 = get_exact_solution( x2, y2);
  double u3 = get_exact_solution( x3, y3);

  double u12 = get_exact_solution( x12, y12);
  double u13 = get_exact_solution( x13, y13);
  double u23 = get_exact_solution( x23, y23);

  double u123 = get_exact_solution( x123, y123);

  double uh1 = U( m->get_global_id( elem, 0));
  double uh2 = U( m->get_global_id( elem, 1));
  double uh3 = U( m->get_global_id( elem, 2));

  double uh12 = average( uh1, uh2);
  double uh13 = average( uh1, uh3);
  double uh23 = average( uh2, uh3);

  double uh123 = average( uh1, uh2, uh3);

  double e1 = (u1 - uh1) * (u1 - uh1);
  double e2 = (u2 - uh2) * (u2 - uh2);
  double e3 = (u3 - uh3) * (u3 - uh3);

  double e12 = (u12 - uh12) * (u12 - uh12);
  double e23 = (u23 - uh23) * (u23 - uh23);
  double e13 = (u13 - uh13) * (u13 - uh13);

  double e123 = (u123 - uh123) * (u123 - uh123);

  double tmp1 = e1  + e2  + e3;
  double tmp2 = e12 + e13 + e23;
  double tmp3 = e123;

  double error = 3.0 * tmp1 + 8.0 * tmp2 + 27 * tmp3;

  error *= area / 60.0;

  return error;
}

void solution::compute_H1_error()
{
  double error = 0.0;
  for( int elem = 0; elem < numElems; elem++)
  {
    error += get_elemental_error_H1( elem);
  }

  H1_error = error + L2_error * L2_error;
  H1_error = std::sqrt( H1_error);

  std::cout << std::scientific
    << "H1 Error = "
    << H1_error
    << std::endl;
  return;
}

double solution::get_elemental_error_H1( int elem)
{
  double area = m->get_elem_area( elem);

  double x1 = m->get_pos( elem, 0, 0);
  double y1 = m->get_pos( elem, 0, 1);

  double x2 = m->get_pos( elem, 1, 0);
  double y2 = m->get_pos( elem, 1, 1);

  double x3 = m->get_pos( elem, 2, 0);
  double y3 = m->get_pos( elem, 2, 1);

  double x12 = average( x1, x2);
  double x13 = average( x1, x3);
  double x23 = average( x2, x3);

  double y12 = average( y1, y2);
  double y13 = average( y1, y3);
  double y23 = average( y2, y3);

  double x123 = average( x1, x2, x3);
  double y123 = average( y1, y2, y3);

  double ux1 = get_exact_solution_grad( x1, y1, 0);
  double ux2 = get_exact_solution_grad( x2, y2, 0);
  double ux3 = get_exact_solution_grad( x3, y3, 0);
  double uy1 = get_exact_solution_grad( x1, y1, 1);
  double uy2 = get_exact_solution_grad( x2, y2, 1);
  double uy3 = get_exact_solution_grad( x3, y3, 1);

  double ux12 = get_exact_solution_grad( x12, y12, 0);
  double ux23 = get_exact_solution_grad( x23, y23, 0);
  double ux13 = get_exact_solution_grad( x13, y13, 0);
  double uy12 = get_exact_solution_grad( x12, y12, 1);
  double uy13 = get_exact_solution_grad( x13, y13, 1);
  double uy23 = get_exact_solution_grad( x23, y23, 1);

  double ux123 = get_exact_solution_grad( x123, y123, 0);
  double uy123 = get_exact_solution_grad( x123, y123, 1);

  double gx1 = (y2 - y3) / (2.0 * area);
  double gy1 = (x3 - x2) / (2.0 * area);

  double gx2 = (y3 - y1) / (2.0 * area);
  double gy2 = (x1 - x3) / (2.0 * area);

  double gx3 = (y1 - y2) / (2.0 * area);
  double gy3 = (x2 - x1) / (2.0 * area);

  double uh1 = U( m->get_global_id( elem, 0));
  double uh2 = U( m->get_global_id( elem, 1));
  double uh3 = U( m->get_global_id( elem, 2));

  double uhx1 = uh1 * gx1;
  double uhx2 = uh2 * gx2;
  double uhx3 = uh3 * gx3;
  double uhy1 = uh1 * gy1;
  double uhy2 = uh2 * gy2;
  double uhy3 = uh3 * gy3;

  double guhx = uhx1 + uhx2 + uhx3;
  double guhy = uhy1 + uhy2 + uhy3;

  double ex1  = (guhx - ux1) * (guhx - ux1); 
  double ey1  = (guhy - uy1) * (guhy - uy1); 
  double ex2  = (guhx - ux2) * (guhx - ux2); 
  double ey2  = (guhy - uy2) * (guhy - uy2); 
  double ex3  = (guhx - ux3) * (guhx - ux3); 
  double ey3  = (guhy - uy3) * (guhy - uy3); 

  double tmp1 = ex1 + ey1 + ex2 + ey2 + ex3 + ey3;

  double ex12  = (guhx - ux12) * (guhx - ux12); 
  double ey12  = (guhy - uy12) * (guhy - uy12); 
  double ex23  = (guhx - ux23) * (guhx - ux23); 
  double ey23  = (guhy - uy23) * (guhy - uy23); 
  double ex13  = (guhx - ux13) * (guhx - ux13); 
  double ey13  = (guhy - uy13) * (guhy - uy13); 

  double tmp2 = ex12 + ey12 + ex13 + ey13 + ex23 + ey23;

  double ex123  = (guhx - ux123) * (guhx - ux123); 
  double ey123  = (guhy - uy123) * (guhy - uy123); 

  double tmp3 = ex123 + ey123;

  double error = 3.0 * tmp1 + 8.0 * tmp2 + 27 * tmp3;

  error *= area / 60.0;

  return error;
}

double solution::get_exact_solution( double x, double y)
{
  double u = 0.0;
  
  if( CaseNumber == 1)
  {
    u = 1.0;
  }
  else if( CaseNumber == 2)
  {
    u = x;
  }
  else if( CaseNumber == 3)
  {
    u = y;
  }
  else if( CaseNumber == 4)
  {
    double a = y * y * y;
    double b = std::sin( 5.0 * (x + y));
    double c = 2.0 * std::exp( x);
    u = a + b + c;
  }
  return u;
}

double solution::get_exact_solution_grad( double x, double y, int xy)
{
  double du = 0.0;

  if( xy == 0)
  {
    if( CaseNumber == 1)
    {
      du = 0.0;
    }
    else if( CaseNumber == 2)
    {
      du = 1.0;
    }
    else if( CaseNumber == 3)
    {
      du = 0.0;
    }
    else if( CaseNumber == 4)
    {
      double a = 0.0;
      double b = 5.0 * std::cos( 5.0 * (x + y));
      double c = 2.0 * std::exp( x);
      du = a + b + c;
    }
  }
  else if ( xy == 1)
  {
    if( CaseNumber == 1)
    {
      du = 0.0;
    }
    else if( CaseNumber == 2)
    {
      du = 0.0;
    }
    else if( CaseNumber == 3)
    {
      du = 1.0;
    }
    else if( CaseNumber == 4)
    {
      double a = 3.0 * y * y;
      double b = 5.0 * std::cos( 5.0 * (x + y));
      double c = 0.0;
      du = a + b + c;
    }
  }
  else
  {
    std::cout 
      << "Unrecognized xy option: "
      << xy
      << std::endl;

    std::abort();
  }

  return du;
}
