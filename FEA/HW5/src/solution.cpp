#include "solution.hpp"


solution::solution( mesh* m_, int CaseNumber_)
{
  m  = m_;
  CaseNumber = CaseNumber_;

  numIntNodes = m->get_number_interior_nodes();

  K = SparseMatrix<double>(numIntNodes, numIntNodes);
  F = VectorXd::Zero( numIntNodes);
  U = VectorXd::Zero( numIntNodes);
}

solution::~solution()
{

}
