// PUMI Headers
#include <PCU.h>
#include <pumi.h>

// STL Headers
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

void Print_Coords_of_Vert(pMeshEnt Vert){
  double Coord[3] = {0,0,0}; 

  std::cout << "Vertex Coordinates: " << std::endl; 
  pumi_node_getCoord(Vert, 0, Coord);
  for(int i =0; i<3; i++)
  {
    std::cout << Coord[i] << "\t";
  }
  std::cout << std::endl;
return;
}


int main(int argc, char** argv)
{
  MPI_Init(&argc,&argv);
  pumi_start();
  pGeom geom = pumi_geom_load(NULL, "null");
  pMesh mesh = pumi_mesh_create(geom, 3);
 
  // Declare working variables
  double Coords[3] = {0,0,0};
  double EdgeLength = 8.0;
  double Height = 4.0;
  double Width = 6.0;
  double dw = EdgeLength/(Width);
  double dh = EdgeLength/(Height);
  int Vert_Width = 1+(int)Width;
  int Vert_Height = 1+(int)Height;
  pMeshEnt Vert;
  std::vector<pMeshEnt> Verts;

  // Create a list to Write to
  std::ofstream Op3Results;
  Op3Results.open("a1_operation3_results.txt");
  Op3Results << "Coordinates of New nodes:\n";
  Op3Results << "X \tY \tZ\n";

  // Create all verteces
  // Loop over points along Height
  for(int i = 0; i<Vert_Height; i++)
  { 
    // Loop over points along width
    for(int j = 0; j<Vert_Width; j++)
    {
      // Define Coordinates for this point
      Coords[0] = j*dw;
      Coords[1] = i*dh;
      // Coords[2] = 0; // Not needed since everything statys in z=0 plane

      // Print Coordinates
      for( int k = 0; k<3; k++)
      {
        Op3Results << Coords[k];
        Op3Results << "\t";
      }
      Op3Results << "\n"; 

      // Create Mesh Vertex at this point
      Vert = pumi_mesh_createVtx( mesh, NULL, Coords);
      Verts.push_back(Vert);
    }
  } 
  
  std::cout << "Finished Verteces Edges" << std::endl;
  std::cout << "Total Verteces placed: " << (int)Verts.size() << std::endl;
  
  pMeshEnt Vert_pair[2];
  std::vector<pMeshEnt> Edges;
  pMeshEnt edge;

  // Create edges between verteces
  // Loop over ordered vector of verteces to create horizontal edges
  for(int i=0; i< (int)Verts.size(); i++)
  {
    if( (i+1)%(Vert_Width) != 0 || i == 0)
    {
      Vert_pair[0] = Verts[i];
      Vert_pair[1] = Verts[i+1];
      // Create edge
      edge = pumi_mesh_createEnt( mesh, NULL, 1, Vert_pair);
      Edges.push_back(edge);
    } // End if
  } // End for
  
  int Hor_Edges = Edges.size();
  std::cout << "Finished Creating " << Hor_Edges << " Horizontal Edges" << std::endl;

  // Loop over ordered vector of verteces to create vertical edges
  for(int i=0; i< (int)Verts.size(); i++)
  {
    if( i < (Vert_Width)*(Height))
    {
      Vert_pair[0] = Verts[i];
      Vert_pair[1] = Verts[i+Vert_Width];
      // Create edge
      edge = pumi_mesh_createEnt( mesh, NULL, 1, Vert_pair);
      Edges.push_back(edge);
    } // End if
  } // End for

  std::cout << "Finished Creating Vertical Edges" << std::endl;
  std::cout << "Total Edges placed: " << (int)Edges.size() << std::endl;
  
  // Create Edge Group to write to 
  pMeshEnt downward[4];
  
  // Create Vector of Elements
  std::vector<pMeshEnt> Elements;
  pMeshEnt element;
  
  // Create Topology
  for( int i = 0; i< (Hor_Edges-(int)Width); i++)
  {
    // Create group of downward adjecent edges
    downward[0] = Edges[i];
    downward[1] = Edges[i+Hor_Edges+1];
    downward[2] = Edges[i+Vert_Width];
    downward[3] = Edges[i+Hor_Edges];
    // Create Element
    element = pumi_mesh_createEnt( mesh, NULL, 3, downward);
    Elements.push_back(element);

  }
  int Tot_Elements = (int)Elements.size();
  std::cout << "Finished Creating Element" << std::endl;
  std::cout << "Total Elements placed: " << Tot_Elements << std::endl;

  std::cout << "Assigning GlobalID\n" << std::endl;
  // Assign Global Numbering scheme to finished mesh
  pumi_mesh_createGlobalID( mesh );
  int ID;

  // Prep printout space for Element and Vertex numbers
  Op3Results << "\nList of Elements and Vertex Members\n" ; 
  Op3Results << "Element Number:\tVerteces on Element:\n" ; 
  
  // Create list of member verteces
  std::vector<pMeshEnt> adj_Verts;
  apf::MeshIterator* ElemIt = mesh->begin(2);
  
  while(( element = mesh->iterate(ElemIt)))
  {
    std::cout << "Getting Element GlobalID\n" << std::endl;
    ID = (pumi_ment_getGlobalID( element ));
    Op3Results << ID ;
    Op3Results << "\t" ;
    // Get dimension 0 (vertex) adjecntcies of element
    pumi_ment_getAdj(element, 0, adj_Verts);
    
    for(int j=0; j< (pumi_ment_getNumAdj(element, 0)); j++)
    {
      Vert = adj_Verts[j];
      std::cout << "Getting Vertex GlobalID \n" << std::endl;
      Op3Results << (pumi_ment_getGlobalID( Vert )) ; 
      Op3Results << "\t" ;
      
    }
    Op3Results << "\n";
  }
  mesh->end(ElemIt);

  // Clean up
  Op3Results.close();
  pumi_mesh_freeze(mesh);
 
  std::cout << "Writing Mesh..." << std::endl; 
  pumi_mesh_write(mesh,"outQuad","vtk");
  std::cout << "Finished Writing Mesh." << std::endl;
  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}

/* Questions for work session:
 * 1. Crashes when writing <mesh>.vtk files; why?
 * 2. Crashes when (attempting to) read GlobalID for elements; why?
 */
