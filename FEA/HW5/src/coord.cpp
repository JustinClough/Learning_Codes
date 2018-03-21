#include "coord.hpp"

#include <iostream>

coord::coord( double x_, double y_)
{
  x = x_;
  y = y_;
}

double coord::get_x()
{
  return x;
}

double coord::get_y()
{
  return y;
}

void coord::print()
{
  std::cout << "( " << x << " , " << y << " )" ;

  return;
}
