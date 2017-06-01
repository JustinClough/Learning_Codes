/*
* Finite Element Analysis code
* Reads geometry and problem definition
* from control file. Assumes:
*   - a rectangular geometry in the XY plane at Z=0. 
*   - all potential dof associate with node points
*   - number of dof per node is the same for all nodes
*
*
*
*/


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
#include <cstdlib>

using std::cout;
using std::endl;
using std::string;

class boundaryCond_t
{
  public:
    char type; // N for Neumann, D for Dirichlet
    int geom_dim;
    int geom_ID;
    int direction;
    double value;
    bool DOG_zero;
    void print();
    boundaryCond_t();
};
class paramList
{
  public:
    int dimension;
    int order;
    int numSides;
    double refinement;
    pGeom geom;
    pMesh mesh;
    std::vector<boundaryCond_t> BCs;
    void assign_BC(boundaryCond_t BonCon)
    { BCs.push_back(BonCon); }
    void print();
};

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

  finish(list);
  return 0;
}

void finish(paramList& list)
{
  pumi_mesh_delete(list.mesh);
  pumi_finalize();
  MPI_Finalize();
  return;
}

void start(int argc, char** argv)
{
  MPI_Init(&argc,&argv);
  pumi_start();
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
    }
    else { print_error("ERROR READING BOUNDARY CONDITION VALUES");}
    action.erase(0, pos+delim.size());
    pos = action.find(delim);
  }

  if(BC.value == 0)
  {
    zero_flag = true;
  }

  if(cmd.compare("NEUMANN")==0)
  {
    BC.type = 'N';
  }
  else if (cmd.compare("DIRICHLET")==0)
  {
    BC.type = 'D';
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

void create_mesh_verts(paramList& list, std::vector<pMeshEnt>& verts)
{
  double refine = list.refinement;
  std::vector<pGeomEnt> geom_corners;
  double coords[3] = {0,0,0};

  gmi_model* gmi_m = (gmi_model*)(list.geom);
  cout << "zero" << endl;
  gmi_iter* it = gmi_begin(gmi_m, 1);
  double p[2] = {0.0, 0.0};
  apf::Vector3 x;
  cout << "one" << endl;
  if(gmi_can_eval(gmi_m))
  {
    gmi_ent* gmi_e = gmi_next(gmi_m, it);
    cout << "two" << endl;
    gmi_eval(gmi_m, gmi_e, p, &x[0]);
    cout << "three" << endl;
  }
  //print_comps(x);
  
  //int numXVerts =  
  //int numYVerts = 
  
  return;
}

void create_elems(paramList& list)
{
  std::vector<pMeshEnt> verts;
  create_mesh_verts(list, verts);
  
  return;
}

void create_mesh(paramList& list)
{
  list.mesh = pumi_mesh_create(list.geom, 2);
  create_elems(list);
  pumi_mesh_freeze(list.mesh);

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
