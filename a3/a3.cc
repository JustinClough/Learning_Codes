//PUMI Headers
#include <PCU.h>
#include <pumi.h>

//STL Headers
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
  MPI_Init(&argc,&argv);
  pumi_start();
  pGeom geom = pumi_geom_load("cube.dmg","mesh");
  pMesh mesh = pumi_mesh_load(geom,"parallelMesh.smb",2);



  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}
