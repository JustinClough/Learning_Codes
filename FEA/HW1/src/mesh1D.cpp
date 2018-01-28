#include "mesh1D.hpp"

#include "iostream"
#include <cstdlib>

mesh1D::mesh1D( double oddSize_, double evenSize_, int NplusOne)
{
  numElems = NplusOne - 1;
  oddSize = oddSize_;
  evenSize = evenSize_;
  constructElems();
}

mesh1D::~mesh1D()
{
  for( int i = 0; i < elements.size(); i++)
  {
    delete elements[i];
  }
}

elem mesh1D::getElem(int i)
{
  if ( i > numElems)
  {
    std::cout
      << "Attempted to access out of bounds element. "
      << "Element number " << i << " requested. "
      << "Maximum number " << numElems << std::endl;
    std::abort();
  }
  return *elements[i];
}

void mesh1D::constructElems()
{
  for( int i = 0; i < numElems+1; i++)
  {
    constructElement( i);
  }
  return;
}

void mesh1D::constructElement( int i)
{
  // First figure out where the last element ended.
  double leftPos =  0.0;
  if ( i != 0)
  {
    leftPos = (elements[i-1])->getRightPos();
  }

  double DX = 1.0 / (numElems + 1.0);
  double rightPos = leftPos;

  // Create element based on even or odd number
  if ( i%2 == 0)
  { // This is an even element.
    rightPos += evenSize * DX;
  }
  else
  { // This is an odd element.
    rightPos += oddSize * DX;
  }

  elem* tmpElem = new elem( leftPos, rightPos);

  elements.push_back( tmpElem);

  return;
}

int mesh1D::getNumElems()
{
  return numElems;
}

int mesh1D::getNumNodes()
{
  return numNodes;
}
