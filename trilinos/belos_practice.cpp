/* Solve Ax=b, random b, Tridiagnol A*/

// Standard Lib Headers
#include <iostream>

// Trilinos Headers
#include "Tpetra_Vector.hpp"
#include <Tpetra_CrsMatrix.hpp>
#include <Tpetra_DefaultPlatform.hpp>
#include <Teuchos_GlobalMPISession.hpp>
#include <Teuchos_oblackholestream.hpp>

// Standard Using Statements
using std::cout;
using std::endl;

// Trillinos Using and Typedef Statements
using Teuchos::RCP;
using Teuchos::rcp;
using Teuchos::tuple;
using Teuchos::Array;
using Teuchos::ArrayRCP;
using Teuchos::ArrayView;
typedef Tpetra::Vector<> vector_t;
typedef Tpetra::Vector<>::scalar_type scalar_t;
typedef Tpetra::Vector<>::global_ordinal_type global_ordinal_t;
typedef Tpetra::Vector<>::local_ordinal_type local_ordinal_t;
typedef Tpetra::CrsMatrix<> crs_mat_t;
typedef Tpetra::Map<> map_t;

RCP<crs_mat_t> make_A(RCP<const map_t> map, int n, std::ostream& out)
{
  // Construct empty A, diagnol and off diag components
  RCP <crs_mat_t> A (new crs_mat_t (map,0));
  scalar_t diag_comp = 2.0;
  scalar_t off_diag_comp = -1.0;
  
  // Get local number of elements
  size_t numMyElems = map->getNodeNumElements();

  // Loop through rows of A and assign values
  for(local_ordinal_t lclRow = 0;lclRow < (local_ordinal_t) numMyElems;++lclRow)
  {
    // Assign Global Row and column Counter 
    //  (same since working on diagnol
    global_ordinal_t gblRow = map->getGlobalElement(lclRow);
    global_ordinal_t gblCol = gblRow;

    // Assign index and values, edge cases first
    if( gblRow == 0)
    { // A(0,0:1) = [diag, off_diag]
      out << "Placed first Row contents" << endl;
      A->insertGlobalValues( gblRow, 
            tuple<global_ordinal_t> (gblCol, gblCol+1), 
            tuple<scalar_t> (off_diag_comp, diag_comp));
    }
    else if( gblRow == n-1)
    { // A(n-1, n-2:n-1) = [off_diag, diag]
      out << "Placed Last Row Contents" << endl;
      A->insertGlobalValues( gblRow, 
            tuple<global_ordinal_t> (gblCol-1, gblCol), 
            tuple<scalar_t> (diag_comp, off_diag_comp));
    }
    else
    { // A(i, i-1:i+1) = [off_diag, diag, off_diag]
      out << "Placed row number " << int(lclRow) << "'s contents" << endl;
      A->insertGlobalValues( gblRow, 
            tuple<global_ordinal_t> (gblCol-1, gblCol, gblCol+1), 
            tuple<scalar_t> (off_diag_comp, diag_comp, off_diag_comp));
    }
  }
  A->fillComplete();
  return A;
}

void solve_Axb( 
      Teuchos::RCP<const Teuchos::Comm<int> >& comm,
      std::ostream& out,int n)
{
  
  Tpetra::global_size_t numGlobalEnts = n;
  global_ordinal_t indexBase = 0;
  RCP<const map_t> map = 
      rcp ( new map_t (numGlobalEnts, indexBase, comm));

  // Create a vector x, initilize with all zeros
  vector_t x (map);
  x.putScalar(0.0);

  // Create a vector b with same map as x, randomize entries
  vector_t b (x, Teuchos::Copy);
  b.randomize(); 

  out << "||x||_2 = " << x.norm2() << endl;
  out << "||b||_2 = " << b.norm2() << endl;

  make_A( map, n, out);
  
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
    cout << "Success!" << endl;
  }

  return 0;
}
