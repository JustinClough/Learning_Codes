#include "mesh1D.hpp"

mesh1D::mesh1D( double oddSize_, double evenSize_, int elems)
{
  numElems = elems;
  oddSize = oddSize_;
  evenSize = evenSize_;
  constructElems();
}

void mesh1D::constructElems()
{

  return;
}
