#include <PCU.h>
#include <pumi.h>
#include <apf.h>
#include <vector>
#include <iterator>
#include <iostream>
#include <stdlib.h>

int main(int argc, char** argv)
{
  //Start
  MPI_Init(&argc,&argv);
  pumi_start();
  pGeom geom = pumi_geom_load("cube.dmg", "mesh");
  pMesh mesh = pumi_mesh_load(geom, "tet-mesh-1.smb",1);

  // Operation 1: Calculate volumes of each mesh region.
  //              List element numbers with corresponding Volume.

  // Begin mesh region iterator and entity pointer
  apf::MeshEntity* region;
  apf::MeshIterator* RegIt = mesh->begin( 3);

 // while((region = mesh->iterate(RegIt)))
 // {
    //Contents here
 // }

  // Operation 2: For each vertex, determine number of bounded edges.


  // Operation 6: Convert Linear elements to Quads.
  //              List Coordinates of new midside nodes.


	//Finish
  pumi_mesh_write(mesh,"outTet","vtk");
  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}
