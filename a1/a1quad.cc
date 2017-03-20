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
  double x = 0;
  double y = 0; 
  double dw = EdgeLength/(Width);
  double dh = EdgeLength/(Height);
  pMeshEnt Vert;
  std::vector<pMeshEnt> Verts;

  // Create a list to Write to
  std::ofstream Op3Results;
  Op3Results.open("a1_operation3_results.txt");
  Op3Results << "Coordinates of New nodes:\n";
  Op3Results << "X \tY \tZ\n";

  // Create all verteces
  // Loop over points along Height
  for(int i = 0; i<Height+1; i++)
  { 
    // Loop over points along width
    for(int j = 0; j<Width+1; j++)
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

  std::cout << "Total Verteces placed: " << (int)Verts.size() << std::endl;
  
  pMeshEnt Vert_pair[2];
  std::vector<pMeshEnt> Edges;
  pMeshEnt edge;

  // Create edges between verteces
  // Loop over ordered vector of verteces to create horizontal edges
  for(int i=0; i< (int)Verts.size(); i++)
  {
    if( (i+1)%((int)(Width+1)) != 0 || i == 0)
    {
      Vert_pair[0] = Verts[i];
      Vert_pair[1] = Verts[i+1];
      // Create edge
      edge = pumi_mesh_createEnt( mesh, NULL, 1, Vert_pair);
      Edges.push_back(edge);
    } // End if
  } // End for

  // Loop over ordered vector of verteces to create vertical edges
  for(int i=0; i< (int)Verts.size(); i++)
  {
    if( i < (Width+1)*(Height))
    {
      Vert_pair[0] = Verts[i];
      Vert_pair[1] = Verts[i+7];
      Print_Coords_of_Vert( Vert_pair[0]);
      // Create edge
      edge = pumi_mesh_createEnt( mesh, NULL, 1, Vert_pair);
      Edges.push_back(edge);
    } // End if
  } // End for

  std::cout << "Finished Creating Vertical Edges" << std::endl;
  
  // Create Edge Group to write to 
  pMeshEnt downward[4];
  
  // Create Topology
  for( int i = 0; i< Width*Height; i++)
  {







  }


  // Clean up
  Op3Results.close();
  pumi_mesh_freeze(mesh);
  
  pumi_mesh_write(mesh,"outQuad","vtk");
  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}

