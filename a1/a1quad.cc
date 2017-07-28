// PUMI Headers
#include <PCU.h>
#include <pumi.h>
#include <apf.h>
#include <apfNumbering.h>

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
  pMesh mesh = pumi_mesh_create(geom, 2);
 
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
  
  std::cout << "Total Verteces placed: " <<(int)Verts.size() << std::endl;

  // Create vertex Group to write to 
  pMeshEnt downward[4];
  
  // Create Vector of Elements
  std::vector<pMeshEnt> Elements;
  pMeshEnt element;
  
  std::cout << "Creating Topology... \n" << std::endl;

  // Create Topology
  for(int i=0; i< ((int)Verts.size()-Vert_Width); i++)
  {
    if( (i+1)%(Vert_Width) != 0 || i == 0)
    {
      downward[0] = Verts[i];
      downward[1] = Verts[i+1];
      downward[2] = Verts[i+Vert_Width+1];
      downward[3] = Verts[i+Vert_Width];
      // Create edge
      element = pumi_mesh_createElem( mesh, NULL, 3, downward);
      Elements.push_back(element);
    } // End if
  } // End for

  int Tot_Elements = (int)Elements.size();
  std::cout << "Finished Creating Element" << std::endl;
  std::cout << "Total Elements placed: " << Tot_Elements << std::endl;
  
  mesh->acceptChanges();
  //pumi_mesh_verify( mesh); //Can't verify before assigning numbering; fails geometric asserts (?)

  // Create Local Numbering Scheme for elements and Vertices
  apf::Numbering* num_elem = numberOwnedDimension( mesh, "Element_Number", 2);
  apf::Numbering* num_vert = numberOwnedDimension( mesh, "Vertex_Number", 0);

  // Prep printout space for Element and Vertex numbers
  Op3Results << "\nList of Elements and Vertex Members\n" ; 
  Op3Results << "Element Number:\tVerteces on Element:\n" ; 
  
  // Create list of member verteces
  std::vector<pMeshEnt> adj_Verts;
  
  int ID = 0;
  for(int i=0; i<(int)Elements.size(); i++ )
  {
    ID = apf::getNumber(num_elem,Elements[i], 0,0);
    Op3Results <<  ID;
    Op3Results << "\t\t" ;
    // Get dimension 0 (vertex) adjcentcies of element
    pumi_ment_getAdj(Elements[i], 0, adj_Verts);
    
    for(int j=0; j< (pumi_ment_getNumAdj(Elements[i], 0)); j++)
    {
      Op3Results <<  apf::getNumber(num_vert, adj_Verts[j], 0,0); 
      Op3Results << "\t" ;
      adj_Verts.clear();
    }
    Op3Results << "\n";
  }

  // Clean up
  Op3Results.close();
  apf::destroyNumbering(num_elem);
  apf::destroyNumbering(num_vert);
  pumi_mesh_freeze(mesh);
  pumi_mesh_verify( mesh);
 
  pumi_mesh_write(mesh,"outQuad","vtk");
  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}
