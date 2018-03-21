#include "vertex.hpp"

#include <cstdlib>
#include <iostream>

vertex::vertex( coord pt_)
{
  pt = new coord( pt_);
  gID = -1;
}

vertex::~vertex()
{
  delete pt;
  elems.clear();
}

void vertex::set_global_ID( int i)
{
  gID = i;
  return;
}

int vertex::get_global_ID()
{
  if ( gID < 0)
  {
    std::cout
      << "Error: Global ID not set for this vertex." << std::endl
      << "This vertex is at ";

    pt->print();

    std::cout 
      << std::endl
      << "Aborting process." << std::endl;
  
    std::abort();
  }

  return gID;
}


void vertex::add_owner( element* elem)
{
  elems.push_back( elem);
  return;
}

void vertex::get_owners( std::vector< element*> &owners)
{
  for ( size_t i = 0; i < elems.size(); i++)
  {
    owners.push_back( elems[i] );
  }
  return;
}
