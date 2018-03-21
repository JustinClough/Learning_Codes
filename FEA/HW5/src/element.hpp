#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <vector>

#include "vertex.hpp"
class vertex;

class element
{
  public: 
    // Constructor, 3 vertices in CCW ordering
    element( std::vector< vertex> vtxs_, int ID);
    
    // Get this element's global ID
    int get_elem_ID();

    // Get the i'th vertex (local ID)
    vertex* get_vertex( int i);

  private:
    // The global element ID
    int elem_ID;

    // Vector of vertixes (CCW)
    std::vector< vertex> vtxs;


};

#endif
