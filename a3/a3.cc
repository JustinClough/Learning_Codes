//PUMI Headers
#include <PCU.h>
#include <pumi.h>

//STL Headers
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <string>
#include <sstream>

using std::cout;
using std::endl;

class collection_t
{
  public:
    std::set<pMeshEnt> set;
    void add_unqs(std::vector<pMeshEnt> ents)
    {// Add element only if unique
      for(int i=0; i<int(ents.size());i++)
      {
        pMeshEnt e = ents[i];
        if(set.count(e)==0)
        {
          set.insert(e);
        }
      }
      return;
    }
    int size()
    { set.size();}
};

template <typename T>
  std::string make_string( T num)
  {
    std::ostringstream ss;
    ss << num;
    return ss.str();
  }

void operation_1(pGeom geom, pMesh mesh)
{ // Determine the number of regions using each mesh vertex accounting for the full mesh (including edges on other parts)
  // For each part, list only the vertices owned by that part with local vertex number and the number of mesh 
  //    regions that vertex bounds (on all parts).

  // Create a container for pointer to mesh regions
  collection_t regions;

  // Loop over mesh vertices, place unique adjacent regions in a set
  pMeshIter it;
  pMeshEnt vertex;
  it = mesh->begin(0);
  while((vertex = mesh->iterate(it)))
  {
    std::vector<pMeshEnt> adj_ents;
    pumi_ment_getAdj(vertex, 3, adj_ents);
    regions.add_unqs(adj_ents);
    adj_ents.clear();
  }
  mesh->end(it);
  
  // Have each process write its results for part 1 to a file
  std::ofstream Op1Results;
  std::string fName_s = "Op1Results_Proc" + make_string(pumi_rank()) +".txt";
  char* fName = &fName_s[0u];
  Op1Results.open( fName);
  Op1Results << "Process " << pumi_rank() << " found " << regions.size() << " regions.\n";

  Op1Results << "\nVertex number owned by this part (G: Global ID, L: Local ID) and number of bounded regions (R):\n";
  Op1Results << "G: \tL:\tR:\n" ;
  it = mesh->begin(0);
  while((vertex = mesh->iterate(it)))
  {
    if(pumi_ment_isOwned(vertex))
    {
      int localID = pumi_ment_getID(vertex);
      int globalID = pumi_ment_getGlobalID(vertex);
      Op1Results << globalID << "\t" ;
      Op1Results << localID << "\t" ;
      int num_adj = pumi_ment_getNumAdj(vertex, 3);
      Op1Results << num_adj << "\n";
    }
  }
  mesh->end(it);
  Op1Results.close();

  return;
} //END operation_1()

void operation_2(pGeom geom, pMesh mesh)
{ // Determine the number of edges using each mesh vertex accounting for the full mesh (included other parts).
  // For each part, list only the vertices owned by that part with local vertex number and the number fo mesh
  //    faces that vertex bounds (on all parts).

  return;
} //END operation_2()

void operation_3(pGeom geom, pMesh mesh)
{ // Count the number of mesh faces on the partition model face before migration.
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
