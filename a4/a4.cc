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
    string type;
    int geom_dim;
    int geom_ID;
    double cond[3];
    bool DOG;
    boundaryCond_t();
};

boundaryCond_t::boundaryCond_t ()
{
  for(int i=0; i<3; i++)
  {
    cond[i] = 0.0;
  }
  DOG = false;
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
  double vector[3] = {0.0,0.0,0.0};

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
  }
  else
  { cout << "Failed to open " << ctrl << endl; }
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
