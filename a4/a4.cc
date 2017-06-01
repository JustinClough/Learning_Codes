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
    double cond[3];
    bool DOG_zero;
    void set_cond(double* vector);
    void print();
    boundaryCond_t();
};

void boundaryCond_t::set_cond(double* vector)
{
  for(int i=0;i<3;i++)
  {
    cond[i] = vector[i];
  }
  return;
}

void boundaryCond_t::print()
{
  cout << "Type = " << type << endl;
  cout << "geom_dim = " << geom_dim << endl;
  cout << "geom_ID = " << geom_ID << endl;
  cout << "Condition Vector = (" ;
  for(int i=0; i<3; i++)
  {
    cout << cond[i] << " " ;
  }
  cout << "\b)" << endl;
  cout << "DOG_zero = " << DOG_zero << endl;
}

boundaryCond_t::boundaryCond_t ()
{
  for(int i=0; i<3; i++)
  {
    cond[i] = 0.0;
  }
  DOG_zero = false;
}

class paramList
{
  public:
    int order;
    int numSides;
    pGeom geom;
    std::vector<boundaryCond_t> BCs;
    void assign_BC(boundaryCond_t BonCon)
    { BCs.push_back(BonCon); }
};

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
    else if (inst>=3 && inst<=5)
    {
      vector[inst-3] = std::atof(value.c_str());
    }
    else { print_error("ERROR READING BOUNDARY CONDITION VALUES");}
    action.erase(0, pos+delim.size());
    pos = action.find(delim);
  }

  if(vector[0]==0 && vector[1]==0 && vector[2] == 0)
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
  BC.set_cond(vector);
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
  else { print_error("ERROR READING CONTROL FILE");}

  return;
}

void parse_control(std::ifstream& file, paramList& list)
{
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
    cout << "Boundary Conditions set: " << endl;
    for(int i=0; i<(int)list.BCs.size(); i++)
    {
      boundaryCond_t BC = list.BCs[i];
      BC.print();
      cout << endl;
    }
  }
  else
  { print_error( "ERROR OPENING CONTROL FILE"); }
  return;
}

int main(int argc, char** argv)
{
  if(argc != 2)
  {
    printf("Usage: %s <Control>.ctrl\n", argv[0]);
    return 0;
  }
  
  MPI_Init(&argc,&argv);
  pumi_start();

  paramList list;

  read_control(argv[1], list);

  pumi_finalize();
  MPI_Finalize();
}
