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
using std::string;

class boundaryCond
{
  public:
    string type;
    int geom_dim;
    int geom_ID;
    double cond[3];
    bool DOG;
    boundaryCond();
};

boundaryCond::boundaryCond ()
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
    string geom_file;
    std::vector<boundaryCond> BCs;
    void assign_BC(boundaryCond BonCon)
    { BCs.push_back(BonCon); }
};

void parse_control(std::ifstream& file, paramList& list)
{
  string line;
  string delim = " ";
  while(std::getline(file, line))
  {
    
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
