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
  //TODO

  return;
}
