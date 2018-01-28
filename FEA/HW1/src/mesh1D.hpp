#ifndef MESH1D_HPP
#define MESH1D_HPP

#include <vector>
#include "element1D.hpp"

// A structure to define and interact with a representative
// one dimensional mesh. Mesh assumed to be structured as 
// defined in homework assignment.
class mesh1D
{
  public:
    // Constructor
    mesh1D( double oddSize, double evenSize, int NplusOne);

    // Destructor
    ~mesh1D();

    // Get the number of elements in the mesh
    int getNumElems();
  
    // Get the number of nodes in the mesh
    int getNumNodes();

    // Get the ith element of the mesh
    elem getElem( int i);

  private:

    // The number of elements in the mesh
    int numElems;
  
    // The number of nodes in the mesh
    int numNodes;

    // The array of elements
    std::vector<elem*> elements; 

    // Element sizes
    double oddSize;
    double evenSize;

    // Constructor of the elements
    void constructElems();

    // Constructor of a single element
    void constructElement( int i);
    
};
#endif
