#include "solution.hpp"

#include <iostream>

solution::solution( mesh* m_, int CaseNumber_)
{
  m  = m_;
  CaseNumber = CaseNumber_;

  p = 3.0;
  q = 2.0;

  elemental_dofs = 3;

  numNodes = m->get_number_nodes();
  numElems = m->get_number_elements();

  K = SparseMatrix<double>(numNodes, numNodes);
  F = VectorXd::Zero( numNodes);
  U = VectorXd::Zero( numNodes);
}

solution::~solution()
{

}

void solution::assemble_problem()
{
  assemble_stiffness();
  assemble_forcing();

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
  // TODO
  return;
}

void solution::assemble_forcing()
{
  //TODO
  for( int i = 0; i < numElems; i++)
  {
    VectorXd f_elem = get_elemental_forcing( i);
    assign_elemental_forcing( f_elem, i);
  }
  return;
}

VectorXd solution::get_elemental_forcing( int elem_num)
{
  VectorXd f_elem = VectorXd::Zero( elemental_dofs);
  // TODO
  return f_elem;
}

void solution::assign_elemental_forcing( VectorXd f_elem, int elem)
{
  // TODO
  return;
}
