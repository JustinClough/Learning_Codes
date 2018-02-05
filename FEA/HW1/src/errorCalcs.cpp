#include "errorCalcs.hpp"

#include <vector>

void errorCalcs( std::vector<VectorXd> solutions, int* NA, int caseNumber)
{
  EC* ec = new EC( solutions, NA, caseNumber);

  ec->create_analytic();
  ec->calc_errors();
  ec->write();
  
  delete ec;
  return;
}

void EC::create_analytic()
{
    // TODO
  return;
}

void EC::calc_errors()
{
  // TODO
  return;
}

void EC::write()
{
  //TODO
  return;
}

EC::EC( std::vector<VectorXd> solutions, int* NA, int caseNumber)
{
  sols = solutions;
  na   = NA;
  cn   = caseNumber;
}
