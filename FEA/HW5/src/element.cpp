#include "element.hpp"


element::element( std::vector< vertex> vtxs_, int ID)
{
  vtxs    = vtxs_;
  elem_ID = ID;
  calc_area();
}

int element::get_elem_ID()
{
  return elem_ID;
}

vertex* element::get_vertex( int i)
{
  return &( vtxs[i]);
}

void element::calc_area()
{
  // TODO
  return;
}
