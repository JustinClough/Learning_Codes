#include "solution.hpp"

#include <iostream>
#include <cstdlib>

Solution::Solution( Mesh* mesh_, 
                    int CaseNumber_, 
                    int method_, 
                    double dt_     )
{

  mesh       = mesh_;
  CaseNumber = CaseNumber_;
  meth       = assign_method( method_);
  dt         = dt_;

}

Solution::~Solution()
{
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

void Solution:: assemble_system()
{
  // TODO
  return;
}

void Solution:: assign_boundary_conditions()
{
  // TODO
  return;
}

void Solution:: solve( double T)
{
  // TODO
  return;
}

void Solution:: calculate_errors()
{
  // TODO
  return;
}

void Solution:: print_data()
{
  // TODO
  return;
}

