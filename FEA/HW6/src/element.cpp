#include "element.hpp"

Element::Element( int L, double xL, int R, double xR)
{
  left  = L;
  right = R;

  left_pos  = xL;
  right_pos = xR;

  calc_length();

  return;
}

void Element::calc_length()
{
  length = right_pos - left_pos;
  return;
}

double Element::get_length()
{
  return length;
}

Element::~Element()
{

}
