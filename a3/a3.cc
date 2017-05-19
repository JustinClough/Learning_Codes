//PUMI Headers
#include <PCU.h>
#include <pumi.h>

//STL Headers
#include <iostream>

using std::cout;
using std::endl;

void operation_1(pGeom geom, pMesh mesh)
{
  // Determine the number of regions using each mesh vertex accounting for th full mesh (including edges on other parts)
  // For each part, list only the vertices owned by that part with local vertex number and the number of mesh 
  //    regions that vertex bounds (on all parts).

  return;
} //END operation_1()

void operation_2(pGeom geom, pMesh mesh)
{
  // Determine the number of edges using each mesh vertex accounting for the full mesh (included other parts).
  // For each part, list only the vertices owned by that part with local vertex number and the number fo mesh
  //    faces that vertex bounds (on all parts).

  return;
} //END operation_2()

void operation_3(pGeom geom, pMesh mesh)
{
  // Count the number of mesh faces on the partition model face before migration.
  // List the mesh regions on Part 1 that have at least one mesh face on teh partition model face between 1 and 2. 
  // Migrate mesh regions from Part 1 to 2. 
  // Count the number of mesh faces on the partition model face after migration.

  return;
} //END operation_3()

int main(int argc, char** argv)
{
  MPI_Init(&argc,&argv);
  pumi_start();
  pGeom geom = pumi_geom_load("cube.dmg","mesh");
  pMesh mesh = pumi_mesh_load(geom,"parallelMesh.smb",2);

  operation_1( geom, mesh);
  operation_2( geom, mesh);
  operation_3( geom, mesh);

  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}
