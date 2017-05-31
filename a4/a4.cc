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


int main(int argc, char** argv)
{
  MPI_Init(&argc,&argv);
  pumi_start();


  pumi_finalize();
  MPI_Finalize();
}
