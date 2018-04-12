#include "element.hpp"

Element::Element( int L, double xL, int R, double xR)
{
  dof_per_elem = 2;

  left  = L;
  right = R;

  left_pos  = xL;
  right_pos = xR;

  calc_length();
  calc_stiffness();
  calc_mass();

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

void Element::calc_stiffness()
{
  stiffness = MatrixXd::Zero( dof_per_elem, dof_per_elem);

  stiffness( 0, 0) = 1.0;
  stiffness( 1, 1) = 1.0;

  stiffness( 0, 1) = -1.0;
  stiffness( 1, 0) = -1.0;

  stiffness /= length;

  return;
}

void Element::calc_mass()
{
  mass = MatrixXd::Zero( dof_per_elem, dof_per_elem);

  mass( 0, 0) = 2.0;
  mass( 1, 1) = 2.0;
  
  mass( 0, 1) = 1.0;
  mass( 1, 0) = 1.0;

  mass *= (length / 6.0);

  return;
}
