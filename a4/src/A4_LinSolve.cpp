#include <BelosBlockGmresSolMgr.hpp>
#include <BelosLinearProblem.hpp>
#include <BelosTpetraAdapter.hpp>
#include <MueLu.hpp>
#include <MueLu_TpetraOperator.hpp>
#include <MueLu_CreateTpetraPreconditioner.hpp>

#include "A4_Control.hpp"
#include "A4_Disc.hpp"
#include "A4_LinAlg.hpp"
#include "A4_LinSolve.hpp"

namespace A4 {

using Teuchos::rcp;
using Teuchos::rcpFromRef;

typedef Tpetra::MultiVector<ST, LO, GO, KNode> MV;
typedef Tpetra::Operator<ST, LO, GO, KNode> OP;
typedef Tpetra::RowMatrix<ST, LO, GO, KNode> RM;
typedef Belos::LinearProblem<ST, MV, OP> LinearProblem;
typedef Belos::SolverManager<ST, MV, OP> Solver;
typedef Belos::BlockGmresSolMgr<ST, MV, OP> GmresSolver;
typedef Tpetra::Operator<ST, LO, GO, KNode> Prec;

static ParameterList get_valid_params() {
  ParameterList p;
  p.set<int>("krylov size", 0);
  p.set<int>("maximum iterations", 0);
  p.set<double>("tolerance", 0.0);
  p.set<int>("output frequency", 0);
  p.sublist("multigrid");
  p.set<std::string>("method", "");
  return p;
}

static ParameterList get_belos_params(ParameterList const& in) {
  ParameterList p;
  int max_iters = in.get<int>("maximum iterations");
  int krylov = in.get<int>("krylov size");
  double tol = in.get<double>("tolerance");
  p.set<int>("Block Size" , 1);
  p.set<int>("Num Blocks", krylov);
  p.set<int>("Maximum Iterations", max_iters);
  p.set<double>("Convergence Tolerance", tol);
  p.set<std::string>("Orthogonalization", "DGKS");
  if (in.isType<int>("output frequency")) {
    int f = in.get<int>("output frequency");
    p.set<int>("Verbosity", 33);
    p.set<int>("Output Style", 1);
    p.set<int>("Output Frequency", f);
  }
  return p;
}


void solve_linear_system(
    ParameterList const& in,
    LinAlg* la,
    Disc* disc)
{
  in.validateParameters(get_valid_params(), 0);
  auto belos_params = get_belos_params(in);
  ParameterList mg_params(in.sublist("multigrid"));
  auto K = la->owned->K;
  auto U = la->owned->U;
  auto F = la->owned->F;
  auto KK = (RCP<OP>)K;
  auto coords = disc->get_coords();
  auto P = MueLu::CreateTpetraPreconditioner(KK, mg_params, coords);
  auto problem = rcp(new LinearProblem(K, U, F));
  problem->setLeftPrec(P);
  problem->setProblem();
  auto solver = rcp(new GmresSolver(problem, rcpFromRef(belos_params)));
  auto dofs = U->getGlobalLength();
  print(" > linear system: num dofs %zu", dofs);
  auto t0 = time();
  solver->solve();
  auto t1 = time();
  auto iters = solver->getNumIters();
  print(" > linear system: solved in %d iterations", iters);
  if (iters >= in.get<int>("maximum iterations"))
    print(" >  WARNING: solve was incomplete!");
  print(" > linear system: solved in %f seconds", t1 - t0);
}

}
