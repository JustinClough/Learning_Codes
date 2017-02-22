#include <PCU.h>
#include <pumi.h>
#include <apf.h>
#include <vector>
#include <iterator>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <apfMesh.h>

int main(int argc, char** argv)
{
  //Start
  MPI_Init(&argc,&argv);
  pumi_start();
  pGeom geom = pumi_geom_load("cube.dmg", "mesh");
  pMesh mesh = pumi_mesh_load(geom, "tet-mesh-1.smb",1);

  // Operation 1: Calculate volumes of each mesh region.
  //              List element numbers with corresponding Volume.

  //Create a list to write to
  std::ofstream Op1Results;
  Op1Results.open("a1_operation1_results.txt");
  Op1Results << "Element Number \tVolume\n";

  //Begin volume counter
  double totalVol = 0.0;
  double thisVol = 0.0;
  //Begin Element Counter
  int eleNumber = 0;

  // Begin mesh region iterator and entity pointer
  apf::MeshEntity* region;
  apf::MeshIterator* RegIt = mesh->begin( 3);
 
  while(( region = mesh->iterate(RegIt)))
  {
    //Get and update information
    eleNumber++;
    thisVol = apf::measure(mesh, region);
    totalVol = totalVol + thisVol;
    //Print Information
    Op1Results << eleNumber;
    Op1Results << "\t";
    Op1Results << thisVol;
    Op1Results << "\n";
  }

  mesh->end(RegIt); 
  //Print total Volume 
  Op1Results << "\n\nTotal Volume: ";
  Op1Results << totalVol;

  //Close Results file
  Op1Results.close(); 

  // Operation 2: For each vertex, determine number of bounded edges.


  //Create a list to write to
  std::ofstream Op2Results;
  Op2Results.open("a1_operation2_results.txt");
  Op2Results << "Vertex Number \tBounded Edges\n";
  // Begin Vertex counter
  int vertCount = 0;
  int upCount = 0;
 
  // Begin mesh vertex pointer and iterator
  apf::MeshEntity* vert;
  apf::MeshIterator* VertIt = mesh->begin(0);

  while(( vert = mesh->iterate(VertIt)))
  {
   //Get and Update Information
   vertCount++;
   upCount = mesh->countUpward(vert); 
   //Print Information
   Op2Results << vertCount;
   Op2Results << "\t";
   Op2Results << upCount;
   Op2Results << "\n";
  }

  Op2Results.close();

  // Operation 6: Convert Linear elements to Quads.
  //              List Coordinates of new midside nodes.

  //Create a list to write to
  std::ofstream Op6Results;
  Op6Results.open("a1_operation6_results.txt");
  Op6Results << "Coordinates of Midside nodes:";
  Op6Results << "X \tY \tZ\n";

  // Create temp vertex to work with
  apf::MeshEntity* tmpVert;

  // Create an edge pointer and iterator
  apf::MeshEntity* edge;
  apf::MeshIterator* EdgeIt = mesh->begin(1);

  while(( edge = mesh->iterate(EdgeIt)))
  {

    //vertex placement here
  
  }

  Op6Results.close();


  // Finish
  pumi_mesh_write(mesh,"outTet","vtk");
  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}
