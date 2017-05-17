// PUMI Headers
#include <PCU.h>
#include <pumi.h>
#include <apfNumbering.h>
#include <apfShape.h>

// STL Headers
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

bool hasNode(pMesh m, pMeshEnt e)
{
  return pumi_shape_hasNode(pumi_mesh_getShape(m),pumi_ment_getTopo(e));
}

double get_sq_magnitude( double* Coords)
{
  double sq_mag = 0.0;
  // Calculate sum of squares of components 
  for (int i=0; i<3; i++)
  {
    sq_mag = sq_mag + (Coords[i])*(Coords[i]); 
  }
  return sq_mag;
}

pMeshEnt find_start( pGeom geom, pMesh mesh)
{
  std::cout << std::endl;
  std::cout << "Finding starting vertex." << std::endl;
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

  std::cout << "Found starting vertex." << std::endl;
  return Start_Vert;
} //END find_start( pGeom geom, pMesh mesh);

void reorder_mesh( pGeom geom, pMesh mesh, pMeshEnt Start_Vert)
{
  std::cout << std::endl;
  std::cout << "Begining Mesh Reorder." << std::endl;
  // Declare working variables
  std::vector<pMeshEnt> q; 
  pMeshEnt vert;
  pMeshEnt edge;
  pMeshEnt node;

  // Get total number of nodes and faces; add 1 for auto-reversing
  pShape shape = pumi_mesh_getShape( mesh );
  int Dof_per_edge = pumi_shape_getNumNode( shape, 1); 
  int labeldof = 1 + pumi_mesh_getNumEnt( mesh, 0) + Dof_per_edge*pumi_mesh_getNumEnt( mesh, 1);
  int labelface = 1 + pumi_mesh_getNumEnt( mesh, 2);

  // Create numbering scheme
  pNumbering num_dofs = createNumbering( mesh, "Degrees_of_Freedom", shape, 1);

  pMeshEnt e;
  pMeshIter it = mesh->begin(0);
  while ((e=mesh->iterate(it))){
    if (isNumbered( num_dofs, e, 0, 1)){
        cout << "Vertex is numbered" << endl;
    }
    cout << getNumber(num_dofs, e, 0,1) << endl;
  }
  
  it = mesh->begin(1);
  while ((e=mesh->iterate(it)))
  {
    if (isNumbered( num_dofs, e, 0, 1)){
        cout << "Line is numbered" << endl;
    }
    cout << getNumber(num_dofs, e, 0,1) << endl;
  }
  
  for (int i=0; i<3; i++)
  {
    if (shape->hasNodesIn(i))
    {
      int num = shape->countNodesOn(i);
      cout << "Has " << num << "  nodes on Dimension "<< i << endl;
    }
  }

  // Begin at starting node
  q.push_back(Start_Vert);

  int LoopCount = 0;
  // Iterate on q until empty, track num of loops
  while( (int)q.size()>0 && LoopCount<=labeldof+1)
  {
    std::cout << "LoopCount = " << ++LoopCount << std::endl;

    // Get the node at front of line; change where the front of the line is
    node = q.front();
    std::cout << "Got node at front of queue." << std::endl;
    q.erase(q.begin());
    std::cout << "Freed front of queue." << std::endl;

    // If this entity has not already been labeled...
    if ( getNumber(num_dofs, node, 0,1)==0 )  // CURRENTLY BREAKS HERE WHEN EVALUATING NODES ON EDGES
    {
      std::cout << "This node has unlabeled DoFs." << std::endl;
      // Then get and label its Degrees of Freedom
      number(num_dofs, node, 0,0,labeldof--);
      cout << "Labeled as " << labeldof << endl;

      // Check if node is from a edge or vertex
      if (getDimension (mesh, node) ==0)
      {
        // Check if mesh edges have nodes
        if (shape->hasNodesIn(1)) 
        {
          cout << "Getting Adjacent Elements" << endl;
          // Get adjacent elements
          apf::Adjacent adj;
          mesh->getAdjacent(node, 2, adj);
          cout << "Number of adjacent faces= " << adj.size() << endl;
          for (int k=0; k< adj.size(); k++)
          {
            cout << "Getting Adjacent Edges" << endl;
            // Get list of edges adjacent to faces
            apf::Adjacent edges;
            mesh->getAdjacent(adj[k], 1, edges);
            cout << "Number of adjacent edges= " << edges.size() << endl;
            // Store edges in the q
            for (int kk=0; kk<edges.size(); kk++)
            {
              cout << "Storing Edge in q." << endl;
              q.insert(q.begin(), edges[kk]);
            }
          }
        }
      }
    }
  }


  std::cout << "Finished Mesh Reorder." << std::endl;
  return; 
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

  // STEP 1: Find starting vertex
  pMeshEnt Start_Vert = find_start( geom, mesh);

  // STEP 2: Reorder Mesh
  reorder_mesh( geom, mesh, Start_Vert);

  // Finish
  pumi_mesh_write(mesh,"numbered","vtk");
  pumi_mesh_delete(mesh);
  pumi_finalize();
  MPI_Finalize();
}
