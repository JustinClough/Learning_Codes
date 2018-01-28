#include "element1D.hpp"

#include <cstdlib>
#include <iostream>

elem::elem( double leftPos_, double rightPos_)
{
  rightPos = rightPos_;
  leftPos = leftPos_;

  length = rightPos - leftPos;

  if ( length <= 0.0)
  {
    std::cout << "Degenerate element construction attempted. "
              << "Attempted length = " 
              << length << std::endl;
    
    std::abort();
  }
}

elem::~elem()
{

}

double elem::getLength()
{
  return length;
}

double elem::getLeftPos()
{
  return leftPos;
}

double elem::getRightPos()
{
  return rightPos;
}
