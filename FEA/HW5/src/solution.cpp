#include "solution.hpp"


solution::solution( mesh* m_, int CaseNumber_)
{
  m  = m_;
  CaseNumber = CaseNumber_;

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

MatrixXd solution::get_elemental_stiffness( int i)
{
  // TODO
  MatrixXd k = MatrixXd::Zero( 1,1);
  return k;
}

void solution::assign_elemental_stiffness( MatrixXd k_elem, int i)
{
  // TODO
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

void solution::assemble_forcing()
{
  //TODO
  return;
}

