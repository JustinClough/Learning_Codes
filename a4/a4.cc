#include "eigen_headers/Sparse"

// PUMI Headers
#include <PCU.h>
#include <pumi.h>

// APF Headers
#include <apfNumbering.h>
#include <apfShape.h>

// GMI Headers
#include "gmi_mesh.h"
#include "gmi_sim.h"

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
    gmi_ent* get_min_mag( );
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

int main( int argc, char** argv)
{

  return 0;
}
