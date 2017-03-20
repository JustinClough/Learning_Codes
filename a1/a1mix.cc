//PUMI headers
#include <PCU.h>
#include <pumi.h>

// STL headers
#include <vector>
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
  MPI_Init(&argc,&argv);
  pumi_start();
  pGeom geom = pumi_geom_load("cube.dmg", "mesh");
  pMesh mesh = pumi_mesh_load(geom, "mixed-mesh-1.smb",1);

  // Create a list to Write to
  std::ofstream Op4Results;
  Op4Results.open("a1_operation4_results.txt");
  Op4Results << "GlobalID's of Mesh Faces classified on Model Face 81:\n";

  // Operation 4: Print list of mesh faces on model face 81
  int modFaceNum = 81; // Model face number according to handout
  pGeomEnt gFace81;

  // Loop over all model faces to find the one global ID of interest
  //    (couldn't find an API to go from globalID to entity)
  for(pGeomIter it=geom->begin(2); it!=geom->end(2); ++it)
  {
    // Get entity with matching globalID
    if(pumi_gent_getID(*it) == modFaceNum)
    {
      gFace81 = *it;
    }
  }

  // Get Mesh faces classified on model face
  std::vector<pMeshEnt> meshFaces;
  pumi_gent_getRevClas( gFace81, meshFaces);
  
  // Loop through the vector of mesh faces; print global_ID
  for(int i=0; i<(int)meshFaces.size(); i++)
  {
    Op4Results << pumi_ment_getGlobalID( meshFaces[i]);
    Op4Results << "\n";
  }

  pumi_mesh_write(mesh,"outMixed","vtk");
  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}
