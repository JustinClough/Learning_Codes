#include "coord.hpp"


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
