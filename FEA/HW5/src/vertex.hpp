#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <vector>

#include "coord.hpp"
#include "element.hpp"
class element;

class vertex
{
  public:
    // Constructor
    vertex( coord pt_);

    // Destructor
    ~vertex();

    // Set this vertex's global ID
    void set_global_ID( int i);

    // Get this vertex's global ID
    int get_global_ID();

    // Set this vertex as a DoF or DoG 
    //  Is a DoF by default
    void set_dof( bool isDof);

    // Get if this vertex is a DoF
    bool get_is_dof();

    // Set this vertex's upward adjacent elements
    void add_owner( element* elem);

    // Get this vertex's upward adjacent elements
    void get_owners( std::vector< element*> &owners);

  private:
    // The coordinates of this vertex
    coord* pt;

    // The upward adjacent owning elements of this vertex
    std::vector< element*> elems;

    // The global ID of this vertex
    int gID;

    // Is a DoF (true) or DoG (false)
    bool is_dof;

};

#endif
