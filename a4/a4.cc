/*
* Finite Element Analysis code
* Reads geometry and problem definition
* from control file. Assumes:
*   - a rectangular geometry in the XY plane at Z=0. 
*   - edges of geometry alinged with coordiate axes
*   - all potential dof associate with node points
*   - number of dof per node is the same for all nodes
*/


// PUMI Headers
#include <PCU.h>
#include <pumi.h>

// APF Headers
#include <apfNumbering.h>
#include <apfShape.h>

// GMI Headers
#include "gmi_mesh.h"
#include "gmi_sim.h"

// Simmetrix Headers
#include <SimUtil.h>

//STL Headers
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <deque>
#include <iterator>
#include <string>
#include <sstream>
#include <cstdlib>
#include <math.h>

using std::cout;
using std::endl;
using std::string;

class Line
{
  public:
    std::deque<pMeshEnt> q;
    std::set<pMeshEnt> set;
    void put_back(pMeshEnt e, pNumbering num)
    {
      if( (set.count(e) == 0) && (!pumi_ment_isNumbered( e, num)))
      {
        q.push_back(e);
        set.insert(e);
      }
    }
    pMeshEnt drop_front()
    {
      pMeshEnt e = q.front();
      q.pop_front();
      set.erase(e);
      return e;
    }
    bool visited( pMeshEnt e, pNumbering num)
    {
      bool isin;
      if(set.count(e) > 0)
      {
        isin = true;
      }
     return (pumi_ment_isNumbered( e, num) || isin);
    }
    void add_list( std::vector<pMeshEnt> list, pNumbering num)
    {
      for(int i=0; i< (int) list.size(); i++)
      {
        put_back(list[i], num);
      }
    }
};

class classification_t
{
  private:
    std::vector<gmi_ent*> ents;
    std::vector<double> mags;
  public:
    void push_back(gmi_ent* ent, double mag);
    void get(int i, gmi_ent* ent, double mag);
    void clear();
    int size();
    void get_min_mag( gmi_ent* ent);
};

class boundaryCond_t
{
  public:
    char type; // N for Neumann, D for Dirichlet
    int geom_dim;
    int geom_ID;
    int direction;
    double value;
    bool DOG_zero;
    bool DOG;
    void print();
    boundaryCond_t();
};
class paramList
{
  public:
    int dimension;
    int order;
    int numSides;
    int numXVerts;
    int numYVerts;
    double refinement;
    pGeom geom;
    gmi_model* gmi_geom;    
    pMesh mesh;
    std::vector<boundaryCond_t> BCs;
    void assign_BC(boundaryCond_t BonCon)
    { BCs.push_back(BonCon); }
    void print();
};

double get_sq_magnitude( double* Coords);
void make_element( paramList& list, pMeshEnt* downward);
void write_mesh( pMesh mesh, const char* name);
double coord_get_mag( double* coord);
void coord_get_diff(double* coord1, double* coord2, double* ans);
void get_bounding_coords(gmi_model* m, double* min, double* max);
void print_test();
void compare_coords(double* a, double* min, double* max);
void print_error( string message);
void create_mesh_verts(paramList& list, std::vector<pMeshEnt>& verts);
void print_comps(double* comps);
void set_BC(string& cmd, string& action,paramList& list);
void line_parse(string& line, size_t& pos, paramList& list);
void parse_control(std::ifstream& file, paramList& list);
void read_control(const char* ctrl, paramList& list);
void create_elems(paramList& list);
void create_mesh(paramList& list);
void start(int argc, char** argv);
void finish(paramList& list);
void reorder_mesh(paramList& list);
pGeomEnt get_ent( paramList& list, double* pos, int dim);

int main(int argc, char** argv)
{
  if(argc != 2)
  {
    printf("Usage: %s <Control>.ctrl\n", argv[0]);
    return 0;
  }

  start(argc, argv);

  paramList list;

  read_control(argv[1], list);
  create_mesh(list);
  reorder_mesh(list);

  finish(list);
  cout << "Success!" << endl;
  return 0;
}

void finish(paramList& list)
{
  pumi_mesh_delete(list.mesh);
  gmi_sim_stop();
  Sim_unregisterAllKeys();
  SimUtil_stop();
  pumi_finalize();
  MPI_Finalize();
  return;
}

void start(int argc, char** argv)
{
  MPI_Init(&argc,&argv);
  pumi_start();
  SimUtil_start();
  Sim_readLicenseFile(0);
  gmi_sim_start();
  gmi_register_mesh();
  gmi_register_sim();
  return;
}

void boundaryCond_t::print()
{
  cout << "Type = " << type << endl;
  cout << "geom_dim = " << geom_dim << endl;
  cout << "geom_ID = " << geom_ID << endl;
  cout << "Direction = " << direction << endl;
  cout << "Value = " << value << endl;
  cout << "DOG_zero = " << DOG_zero << endl;
}

boundaryCond_t::boundaryCond_t ()
{
  DOG = false;
  DOG_zero = false;
}

void paramList::print()
{
  cout << "Dimension = " << dimension << endl;
  cout << "Element Order = " << order << endl;
  cout << "Element Sides = " << numSides << endl;
  cout << "Mesh Refinement Level = " << refinement << endl;
  cout << "Boundary Conditions set: " << endl;
  for(int i=0; i<(int)BCs.size(); i++)
  {
    boundaryCond_t BC = BCs[i];
    BC.print();
    cout << endl;
  }
  return;
}

void print_error( string message)
{
  cout << message << endl;
  std::abort();
}

void set_BC(string& cmd, string& action,paramList& list)
{
  int geom_dim;
  int geom_ID;
  bool zero_flag = false;
  double vector[3] = {0.0,0.0,0.0};
  boundaryCond_t BC;

  string delim = " ";
  size_t pos = action.find(delim);
  size_t null_pos = std::string::npos;
  int inst = 0;
  while(pos!=null_pos)
  {
    string value = action.substr(0, pos);
    inst++;
    if(inst==1)
    {
      geom_dim=std::atoi(value.c_str());
    }
    else if (inst==2)
    {
      geom_ID=std::atoi(value.c_str());
    }
    else if (inst==3)
    {
      if(value.compare("X")==0)
      {
        BC.direction = 0;
      }
      else if(value.compare("Y")==0)
      {
        BC.direction = 1;
      }
      else if(value.compare("Z")==0)
      {
        BC.direction = 2;
      }
      else { print_error("ERROR READING BOUNDARY CONDITION DIRECTION");}
    }
    else if (inst==4)
    {
      BC.value = std::atof(value.c_str());
      if(BC.value == 0)
      {
        zero_flag = true;
      }
    }
    else { print_error("ERROR READING BOUNDARY CONDITION VALUES");}
    action.erase(0, pos+delim.size());
    pos = action.find(delim);
  }


  if(cmd.compare("NEUMANN")==0)
  {
    BC.type = 'N';
  }
  else if (cmd.compare("DIRICHLET")==0)
  {
    BC.type = 'D';
    BC.DOG = true;
  }
  else { print_error("ERROR READING BOUNDARY CONDITION TYPE");}

  BC.geom_dim = geom_dim;
  BC.geom_ID = geom_ID;
  BC.DOG_zero = zero_flag;
  list.assign_BC(BC);
  return;
}

void line_parse(string& line, size_t& pos, paramList& list)
{
  string cmd = line.substr(0, pos);
  string action = line.substr(pos+1, line.length()); 
  if(cmd.compare("ELEMENT_ORDER")==0)
  {
    if(action.compare("linear")==0)
    {
      list.order = 1;
    }
    else if (action.compare("quadratic"))
    {
      list.order = 2;
    }
    else { print_error("ERROR READING ELEMENT_ORDER"); }
  }
  else if (cmd.compare("ELEMENT_SHAPE") ==0)
  {
    if(action.compare("three_sided")==0)
    {
      list.numSides = 3;
    }
    else if(action.compare("four_sided") ==0)
    {
      list.numSides = 4;
    }
    else { print_error("ERROR READING ELEMENT_SHAPE"); }
  }
  else if (cmd.compare("GEOM_FILE")==0)
  {
    list.geom = pumi_geom_load(action.c_str(), "mesh");
  }
  else if (cmd.compare("SIM_GEOM_FILE")==0)
  {
    list.gmi_geom = gmi_sim_load( 0, action.c_str());
  }
  else if (cmd.compare("NEUMANN")==0 || cmd.compare("DIRICHLET")==0)
  {
    set_BC(cmd, action, list);
  }
  else if (cmd.compare("ELEMENT_SIZE")==0)
  {
    list.refinement = std::atof(action.c_str());
  }
  else if (cmd.compare("#")==0)
  {
    // is a commented line, do nothing
  }
  else { print_error("ERROR READING CONTROL FILE");}

  return;
}

void parse_control(std::ifstream& file, paramList& list)
{ 
  list.dimension = 2; // Hard code to only solve 2D problems
  string line;
  string delim = " ";
  while(std::getline(file, line))
  {
    string cmd;
    size_t pos = line.find(delim);
    size_t null_pos = std::string::npos;
    if(pos!=null_pos)
    {
      line_parse(line, pos, list);
    }
  }
  return;
}

void read_control(const char* ctrl, paramList& list)
{
  std::ifstream ctrlFile (ctrl);
  if (ctrlFile.is_open())
  {
    parse_control(ctrlFile, list);
    ctrlFile.close();
  }
  else
  { print_error( "ERROR OPENING CONTROL FILE"); }
  return;
}

void compare_coords(double* a, double* min, double* max)
{
  static bool called = false;
  if (!called)
  {
    min = a;
    max = a;
    called = true;
  }
  else
  {
    for(int i=0; i<3; i++)
    {
      if(a[i]<min[i])
      {
        min[i] = a[i];
      }
      if(a[i]>max[i])
      {
        max[i] = a[i];
      }
    }
  }
  return;
}

void get_bounding_coords(gmi_model* m, double* min, double* max)
{
  gmi_iter* it = gmi_begin( m, 0);
  gmi_ent* g_ent;
  if(gmi_can_eval(m))
  { double p[2] = {0.0, 0.0};
    double x[3] = {0.0, 0.0, 0.0};
    while((g_ent=gmi_next(m,it)))
    {
      gmi_eval( m, g_ent, p, x);
      compare_coords(x, min, max);
    }
  }
  else
  { print_error("CANNOT EVALUATE MODEL"); }
  gmi_end(m, it);
  return;
}

pGeomEnt get_ent( paramList& list, double* pos, int dim)
{
  gmi_ent* gmiEnt;
  gmi_model* model = list.gmi_geom;
  gmi_iter* it = gmi_begin( model, dim);
  double to[] = {0.0, 0.0, 0.0};
  double to_p[] = {0.0,0.0};
  classification_t classif;
  while((gmiEnt = gmi_next( model, it)))
  {
    double ans[] = {0.0, 0.0, 0.0};
    double res[] = {0.0, 0.0, 0.0};
    double diff[] = {0.0, 0.0, 0.0};
    gmi_closest_point(model, gmiEnt, pos, to, to_p);
    gmi_eval(model, gmiEnt, to_p, res);
    coord_get_diff(pos, res, diff); 
    double mag = coord_get_mag(diff);
    classif.push_back(gmiEnt, mag);
  }
  gmi_end(model, it);
  gmi_ent* answer_gmiEnt;
  classif.get_min_mag(answer_gmiEnt);
  classif.clear();
  
  return (list.geom)->getGeomEnt( dim, answer_gmiEnt);
}

void create_mesh_verts(paramList& list, std::vector<pMeshEnt>& verts)
{
  double min[] = {0.0, 0.0, 0.0};
  double max[] = {0.0, 0.0, 0.0};
  gmi_model* m = list.gmi_geom;
  get_bounding_coords(m, min, max);

  double refine = list.refinement;
  double length = max[0] - min[0];
  double height = max[1] - min[1];
  double spacing_x = length*refine;
  double spacing_y = height*refine;
  int numXVerts = (int)ceil(length/spacing_x);
  int numYVerts = (int)ceil(height/spacing_y);
  // Add one since 'one vert needed for space of zero width'
  numXVerts++;  
  numYVerts++;
  list.numXVerts = numXVerts;
  list.numYVerts = numYVerts; 

  pMeshEnt vert;
  int dim;
  for(int i=0; i<numXVerts; i++)
  {
    for(int j=0; j<numYVerts; j++)
    {
      double x_pos = i*spacing_x;
      double y_pos = j*spacing_y;
      double z_pos = 0.0;
      double position[] = {x_pos, y_pos, z_pos};

      // Check for geom point or edge cases
      if(i==0) // left edge
      {
        if(j==0) // bottom point
        {
          dim = 0;
          pGeomEnt g_ent = get_ent( list,  position, dim);
          vert = pumi_mesh_createVtx( list.mesh, g_ent , position);
        }
        else if(j==(numYVerts-1)) // top point
        {
          dim = 0;
          pGeomEnt g_ent = get_ent( list,  position, dim);
          vert = pumi_mesh_createVtx( list.mesh, g_ent , position);
        }
        else // anywhere else on the line
        {
          dim = 1;
          pGeomEnt g_ent = get_ent( list,  position, dim);
          vert = pumi_mesh_createVtx( list.mesh, g_ent , position);
        }
      }
      else if( i==(numXVerts-1)) // right edge
      {
        if(j==0) // bottom point
        {
          dim = 0;
          pGeomEnt g_ent = get_ent( list,  position, dim);
          vert = pumi_mesh_createVtx( list.mesh, g_ent , position);
        }
        else if(j==(numYVerts-1)) // top point
        {
          dim = 0;
          pGeomEnt g_ent = get_ent( list,  position, dim);
          vert = pumi_mesh_createVtx( list.mesh, g_ent , position);
        }
        else // anywhere else on the line
        {
          dim = 1;
          pGeomEnt g_ent = get_ent( list,  position, dim);
          vert = pumi_mesh_createVtx( list.mesh, g_ent , position);
        }
      }
      else if ( j== 0 ) // bottom edge, not including end points
      {
        dim = 1;
        pGeomEnt g_ent = get_ent( list,  position, dim);
        vert = pumi_mesh_createVtx( list.mesh, g_ent , position);
      }
      else if ( j== (numYVerts-1)) // top edge, not including end points
      {
        dim = 1;
        pGeomEnt g_ent = get_ent( list,  position, dim);
        vert = pumi_mesh_createVtx( list.mesh, g_ent , position);
      }
      else // somewhere on the face
      {
        dim = 2;
        pGeomEnt g_ent = get_ent( list,  position, dim);
        vert = pumi_mesh_createVtx( list.mesh, g_ent , position);
      }

      verts.push_back(vert);
    }
  }

  return;
}

void create_elems(paramList& list)
{
  std::vector<pMeshEnt> verts;
  create_mesh_verts(list, verts);
  
  for(int j=0; j<list.numYVerts-1;j++)
  {
    for(int i=0; i<list.numXVerts-1; i++)
    {
      pMeshEnt downward[4];
      int base = i+j*list.numXVerts;
      downward[0] = verts[base];
      downward[1] = verts[base+1];
      downward[2] = verts[base+list.numXVerts+1];
      downward[3] = verts[base+list.numXVerts];
      make_element(list, downward);
    }
  }
  
  return;
}

void create_mesh(paramList& list)
{
  list.mesh = pumi_mesh_create(list.geom, 2);
  create_elems(list);
  pumi_mesh_freeze(list.mesh);
  write_mesh( list.mesh, "mesh");
  return;
}

void print_comps(double* comps)
{
  cout << "(";
  for(int i=0; i<3; i++)
  {
    cout << comps[i] << " " ;
  }
  cout << "\b)\n" << endl;
  return;
}

void print_test()
{
  cout << "Hello World" << endl;
  return;
}

double coord_get_mag( double* coord)
{
  double ans = 0.0;
  for(int i=0; i<3; i++)
  {
    ans += (coord[i]*coord[i]);
  }
  ans = sqrt(ans);
  return ans;
}

void coord_get_diff(double* coord1, double* coord2, double* ans)
{
  for(int i=0; i<3; i++)
  { 
    ans[i] = coord1[i] - coord2[i];
  }
  return;
}

void classification_t::push_back( gmi_ent* ent, double mag)
{
  ents.push_back(ent);
  mags.push_back(mag);
  return;
}

void classification_t::get(int i, gmi_ent* ent, double mag)
{
  ent = ents[i];
  mag = mags[i];
  return;
}

void classification_t::clear()
{
  ents.clear();
  mags.clear();
  return;
}

int classification_t::size()
{
  int s1 = ents.size();
  int s2 = mags.size();
  if(s1 == s2)
  {
    return s1;
  }
  else
  {
    print_error("ERROR: CLASSIFICATION LENGTH MIS-MATCH");
    return 0;
  }
}

void classification_t::get_min_mag( gmi_ent* ent)
{
  gmi_ent* min_ent;
  double min_mag;
  get(0, min_ent, min_mag);
  gmi_ent* ent_now;
  double mag_now;
  for(int i=0;i<size(); i++)
  {
    get(i, ent_now, mag_now);
    if (mag_now<min_mag)
    {
      min_mag = mag_now;
      min_ent = ent_now;
    }
  }
  ent = min_ent;
  return;
}
void write_mesh(pMesh mesh, const char* name)
{
  cout << "Number of Vertices: " << pumi_mesh_getNumEnt(mesh, 0) << "\n";
  cout << "Number of Edges: " << pumi_mesh_getNumEnt(mesh, 1) << "\n";
  cout << "Number of Faces: " << pumi_mesh_getNumEnt(mesh, 2) << "\n";

  pumi_mesh_write( mesh, name, "vtk");
  return;
}

void make_element( paramList& list, pMeshEnt* downward)
{
  pMeshEnt ent;
  pGeomIter it = (list.geom)->begin(2);
  pGeomEnt g_ent = *it;
  if(list.numSides == 3)
  {
    for (int i=0; i<2; i++)
    {
    pMeshEnt down[] = {downward[0], downward[1+i], downward[2+i]};
    pumi_mesh_createElem(list.mesh, g_ent, PUMI_TRIANGLE, down);
    }
  }
  else if (list.numSides == 4)
  {
    pumi_mesh_createElem(list.mesh, g_ent, PUMI_QUAD, downward);
  } 
  else 
  { print_error("ERROR: UNDEFINED NUMBER OF ELEMENT SIDES");}

  return;
}

void reorder_mesh(paramList& list)
{

  return;
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
