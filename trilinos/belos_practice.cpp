// Standard Lib Headers
#include <iostream>

// Trilinos Headers
#include "Tpetra_Vector.hpp"
#include "Tpetra_MultiVector.hpp"
#include "Tpetra_Map.hpp"
#include <Tpetra_CrsMatrix.hpp>
#include <Tpetra_DefaultPlatform.hpp>
#include <MatrixMarket_Tpetra.hpp>

#include <Teuchos_GlobalMPISession.hpp>
#include <Teuchos_oblackholestream.hpp>
#include <Teuchos_ParameterList.hpp>

#include <BelosLinearProblem.hpp>
#include <BelosTpetraAdapter.hpp>
#include <BelosBlockCGSolMgr.hpp>

#include <Phalanx_KokkosDeviceTypes.hpp>

#include <Ifpack2_Factory.hpp>

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

typedef Tpetra::Vector<>                                  vector_t;
typedef Tpetra::Vector<>::scalar_type                     scalar_t;
typedef Tpetra::Vector<>::global_ordinal_type             global_ordinal_t;
typedef Tpetra::Vector<>::local_ordinal_type              local_ordinal_t;
typedef Teuchos::ParameterList                            ParameterList;
typedef Kokkos::Compat::KokkosDeviceWrapperNode
  <PHX::Device>                                           KNode;
typedef Tpetra::MultiVector
  <scalar_t, local_ordinal_t, global_ordinal_t, KNode>    multiVec_t;
typedef Tpetra::Operator
  <scalar_t, local_ordinal_t, global_ordinal_t, KNode>    Operator_t;
typedef Tpetra::CrsMatrix<>                               crs_mat_t;
typedef Belos::SolverManager
  <scalar_t, multiVec_t, Operator_t>                      Solver;
typedef Belos::BlockCGSolMgr
  <scalar_t, multiVec_t, Operator_t>                      CGSolver;
typedef Tpetra::Map<>                                     map_t;
typedef Belos::LinearProblem
  <scalar_t, multiVec_t, Operator_t>                      LinearProblem;
typedef Tpetra::RowMatrix
  <scalar_t, local_ordinal_t, global_ordinal_t, KNode>    row_mat_t;
typedef Ifpack2::Factory                                  factory_t;
typedef Tpetra::MatrixMarket::Writer<crs_mat_t>           MM_Writer;

// Create a tridiagnol matrix
RCP<crs_mat_t> make_A(RCP<const map_t> map, int n)
{ // Construct empty A, diagnol and off diag components
  RCP <crs_mat_t> A (new crs_mat_t (map,0));
  scalar_t diag_comp = 2.0;
  scalar_t off_diag_comp = -1.0;
  // Get local number of elements
  size_t numMyElems = map->getNodeNumElements();
  // Loop through rows of A and assign values
  for(local_ordinal_t lclRow = 0;lclRow < (local_ordinal_t) numMyElems;++lclRow)
  {// Assign Global Row and Column Counter (same since working on diagnol)
    global_ordinal_t gblRow = map->getGlobalElement(lclRow);
    global_ordinal_t gblCol = gblRow;
    // Assign index and values, edge cases first
    if( gblRow == 0)
    { // A(0,0:1) = [diag, off_diag]
      A->insertGlobalValues( gblRow, 
          tuple<global_ordinal_t> (gblCol , gblCol+1), 
          tuple<scalar_t> ( diag_comp , off_diag_comp));
    }
    else if( gblRow == n-1)
    { // A(n-1, n-2:n-1) = [off_diag, diag]
      A->insertGlobalValues( gblRow, 
          tuple<global_ordinal_t> (gblCol-1, gblCol), 
          tuple<scalar_t> (off_diag_comp, diag_comp));
    }
    else
    { // A(i, i-1:i+1) = [off_diag, diag, off_diag]
      A->insertGlobalValues( gblRow, 
          tuple<global_ordinal_t> (gblCol-1, gblCol, gblCol+1), 
          tuple<scalar_t> (off_diag_comp, diag_comp, off_diag_comp));
    }
  }
  A->fillComplete();
  MM_Writer::writeSparseFile("matrix_A", A);
  return A;
}

void CG_solver(
  RCP<ParameterList> pList,
  RCP<LinearProblem> prob,
  std::ostream& out)
{
  prob->setProblem();
  RCP<Solver> solver = rcp(new CGSolver(prob, pList));
  solver->solve();
  out << "Tolerance = " << solver->achievedTol() << endl;
  return;
}

RCP<ParameterList> make_ParamList() 
{
  RCP<ParameterList> list = rcp(new ParameterList);
  int krylov_size = 100;
  int max_iters = 100;
  double tol = 1e-4;
  double abs_thres = 1.0;
  int verbosity = 33;
  int out_style = 1;
  int block_size = 1;
  int out_freq = 1;
  list->set<int>("Num Blocks", krylov_size);
  list->set<int>("Block Size", block_size);
  list->set<int>("Maximum Iterations", max_iters);
  list->set<int>("Verbosity", verbosity);
  list->set<int>("Output Style", out_style);
  list->set<int>("Output Frequency", out_freq);
  list->set<double>("Convergence Tolerance", tol);
  list->set<double>("asdfasdfasdf", 021.2);
  list->set<double>("fact: absolute threshold", abs_thres);
  return list;
}

void solve_Axb( 
    Teuchos::RCP<const Teuchos::Comm<int> >& comm,
    std::ostream& out,
    int n)
{
  Tpetra::global_size_t numGlobalEnts = n;
  global_ordinal_t indexBase = 0;
  RCP<const map_t> map = 
    rcp ( new map_t (numGlobalEnts, indexBase, comm));

  // Create a vector x, initilize with all zeros
  RCP<vector_t> x = rcp(new vector_t(map));
  x->putScalar(0.0);
  // Create a vector b, randomize entries
  RCP<vector_t> b = rcp(new vector_t(map));
  b->putScalar(1.0);
  //b->randomize(); 

  // Print norms 
  out << "||x||_2 = " << x->norm2() << endl;
  out << "||b||_2 = " << b->norm2() << endl;

  // Create matrix A
  RCP<crs_mat_t> A = make_A( map, n);
  out << "Matrix A constructed and filled" << endl;

  // Construct problem and solve
  auto prob = rcp(new LinearProblem(A,x,b));
  RCP<ParameterList> pList = make_ParamList();
  CG_solver( pList, prob, out);

  MM_Writer::writeDenseFile("vector_b", b);
  MM_Writer::writeDenseFile("vector_x", x);

  return;
}


int main(int argc, char** argv)
{
  // Size of system to solve (A is nxn)
  int n = 5;

  // Set MPI communicator and printing
  Teuchos::oblackholestream blackHole;
  Teuchos::GlobalMPISession mpiSession (&argc, &argv, &blackHole);
  RCP<const Teuchos::Comm<int> > comm = 
    Tpetra::DefaultPlatform::getDefaultPlatform().getComm();
  int myRank = comm->getRank();
  std::ostream& out = (myRank==0) ? std::cout : blackHole;

  // Create and solve system
  solve_Axb(comm, out, n);

  if (myRank==0)
  {
    cout << "Success!" << endl;
  }
  return 0;
}
