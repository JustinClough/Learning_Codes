/* Solve Ax=b, random b, Tridiagnol A*/

// Standard Lib Headers
#include <iostream>

// Trilinos Headers
#include "Tpetra_Vector.hpp"
#include <Tpetra_DefaultPlatform.hpp>
#include <Teuchos_GlobalMPISession.hpp>
#include <Teuchos_oblackholestream.hpp>

// Standard Using Statements
using std::cout;
using std::endl;

// Trillinos Using and Typedef Statements
using Teuchos::RCP;
using Teuchos::rcp;
using Teuchos::Array;
using Teuchos::ArrayView;
typedef Tpetra::Vector<>::scalar_type scalar_t;
typedef Tpetra::Vector<>::global_ordinal_type global_ordinal_t;
typedef Tpetra::Map<> map_t;

void solve_Axb( 
      Teuchos::RCP<const Teuchos::Comm<int> >& comm,
      std::ostream& out
      int n)
{
  
  Tpetra::global_size_t numGlobalEnts = n;
  global_ordinal_t indexBase = 0;
  RCP<const map_t> contigMap1 = 
      rcp ( new map_t (numGlobalEnts, indexBase, comm));



  return;
}




int main(int argc, char** argv)
{
  // Size of system to solve (A is nxn)
  int n = 100;

  // Set MPI communicator and printing
  Teuchos::oblackholestream blackHole;
  Teuchos::GlobalMPISession mpiSession (&argc, &argv, &blackHole);
  RCP<const Teuchos::Comm<int> > comm = 
      Tpetra::DefaultPlatform::getDefaultPlatform().getComm();
  int myRank = comm->getRank();
  std::ostream& out = (myRank==0) ? std::cout : blackHole;

  solve_Axb(comm, out, n);


  if (myRank==0)
  {
    cout << n <<  " Success!" << endl;
  }

  return 0;
}
