// PUMI Headers
#include <PCU.h>
#include <pumi.h>

// STL Headers
#include <stdlib.h>
#include <string.h>

bool hasNode(pMesh m, pMeshEnt e)
{
  return pumi_shape_hasNode(pumi_mesh_getShape(m),pumi_ment_getTopo(e));
}

int main(int argc, char** argv)
{
  if (argc != 3) {
    printf("usage: %s reorder_?.dmg reorder_?.smb\n", argv[0]);
    return 0;
  }

  MPI_Init(&argc,&argv);
  pumi_start();
  pGeom geom = pumi_geom_load(argv[1],"mesh");
  pMesh mesh = pumi_mesh_load(geom,argv[2],1);
  if(!strcmp (argv[1], "reorder_c.dmg"))
    pumi_mesh_setShape(mesh,pumi_shape_getLagrange(2));
/*
//// STEP 1:
  // Find most extreme vertex
  std::vector<pMeshEnt> Verts;
  int length_old = 0;
  int length_now = 1;
  while( length_old<length_now)
  {


  }
  
*/



  // Finish
  pumi_mesh_write(mesh,"numbered","vtk");
  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}

