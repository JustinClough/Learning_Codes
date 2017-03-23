// PUMI Headers
#include <PCU.h>
#include <pumi.h>

// STL Headers
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>

bool hasNode(pMesh m, pMeshEnt e)
{
  return pumi_shape_hasNode(pumi_mesh_getShape(m),pumi_ment_getTopo(e));
}

double get_sq_magnitude( double* Coords)
{
  double sq_mag = 0.0;
  // find sum of squares of components 
  for (int i=0; i<3; i++)
  {
    sq_mag = sq_mag + (Coords[i])*(Coords[i]); 
  }
  return sq_mag;
}

pMeshEnt find_start( pGeom geom, pMesh mesh)
{
  // Declare working variables
  std::vector<pMeshEnt> model_Verts;
  std::vector<pMeshEnt> Verts;
  pMeshEnt Start_Vert;
  double Coords[3] = {0,0,0};
  double Sum_Coords[3] = {0,0,0}; 
  double Ave_Coords[3] = {0,0,0};
  double Dif_Coords[3] = {0,0,0};
  double distance = 0.0;
  double g_distance = 0.0;
  double n = 0;

  // For every model vertex...
  for(pGeomIter it=geom->begin(0); it!=geom->end(0); ++it)
  {
    // Get mesh vertex classified on model vertex
    pumi_gent_getRevClas( *it, model_Verts);
    Verts.push_back(model_Verts[0]);

    // Update vertex set stats
    pumi_node_getCoord( model_Verts[0], 0, Coords);
  
    // Clear vector for reuse
    model_Verts.clear();

    for(int i=0; i<3; i++)
    {
      Sum_Coords[i] = Coords[i];
    }
    n = n + 1.0;
  }

  // Get average location of all model vertices
  for(int i=0; i<3; i++)
  {
    Ave_Coords[i] = Sum_Coords[i]/n;
  }

  // For every mesh vertex on model 
  for( int i=0; i< (int)Verts.size(); i++)
  {
    // Get this Vertex's Coordinates
    pumi_node_getCoord( Verts[i], 0, Coords);

    // Compare this vertex's coordinates to the average; get the distance
    for(int j=0; j<3; j++)
    {
      Dif_Coords[j] = Ave_Coords[j] - Coords[j];
    } 
    distance = get_sq_magnitude( Dif_Coords );

    // If this vertex is further from the center of all vertices than all others
    //  (ties go to the home team, not runner)
    if( distance > g_distance)
    {
      // Then, it is likely to be the most extreme vertex
      Start_Vert = Verts[i];
      // Update distance to beat
      g_distance = distance;
    } // ENDIF 
  } // ENDFOR every vertex on the model vertices

  // Clean up
  Verts.clear();

  return Start_Vert;
} //END find_start( pGeom geom, pMesh mesh);


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

  // STEP 1:  Find starting vertex
  pMeshEnt Start_Vert = find_start( geom, mesh);
  std::cout << "Found starting vertex." << std::endl;

  // Finish
  pumi_mesh_write(mesh,"numbered","vtk");
  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}

