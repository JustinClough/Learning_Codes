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
  Op4Results.close();

  // Operation 5: List region type (tet, hex, ...) 
  //              for each mesh region

  // Create a list to Write to
  std::ofstream Op5Results;
  Op5Results.open("a1_operation5_results.txt");
  Op5Results << "Region ID:\tType:\n";
  
  int type;
  pMeshEnt e;
  pMeshIter it = mesh->begin(3);
  while((e = mesh->iterate(it)))
  {
    // Print mesh region global ID
    Op5Results << pumi_ment_getGlobalID(e);
    Op5Results << "\t\t";

    // Get enumeration
    type = pumi_ment_getTopo(e);

    // Convert enumeration to text and print
    if(type == 3)
    {
    Op5Results << "Quad";
    }
    else if(type == 4)
    {
    Op5Results << "Tet";
    }
    else if(type == 5)
    {
    Op5Results << "Hex";
    }
    else if(type == 6)
    {
    Op5Results << "Prism";
    }
    else if(type == 7)
    {
    Op5Results << "Pyramid";
    }
  Op5Results << "\n";
  }
  mesh->end(it);
  Op5Results.close();

  pumi_mesh_write(mesh,"outMixed","vtk");
  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}
