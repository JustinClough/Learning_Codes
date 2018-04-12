#include "element.hpp"

Element::Element( int L, double xL, int R, double xR)
{
  left  = L;
  right = R;

  left_pos  = xL;
  right_pos = xR;

  return;
}

Element::~Element()
{

}
