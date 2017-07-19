#include <apf.h>
#include <apfMesh2.h>
#include <Teuchos_ParameterList.hpp>

#include "A4_Disc.hpp"
#include "A4_FESolver.hpp"
#include "A4_LinSolve.hpp"
#include "A4_ElasticStiffness.hpp"
#include "A4_DBCs.hpp"
#include "A4_NBCs.hpp"
#include "A4_PostProc.hpp"

namespace A4 {

static ParameterList get_valid_params()
{
  ParameterList p;
  p.set<double>("E", 0.0);
  p.set<double>("nu", 0.0);
  p.sublist("dirichlet bcs");
  p.sublist("traction bcs");
  p.sublist("linear algebra");
  return p;
}

FESolver::FESolver(Disc* d, ParameterList const& p)
    : disc(d),
      params(p),
      la(d)
{
  params.validateParameters(get_valid_params(), 0);
}

FESolver::~FESolver()
{
}

void FESolver::assemble_LHS()
{

  // get the mesh and problem parameters
  auto mesh = disc->get_apf_mesh();
  auto int_order = 2;
  double E = params.get<double>("E");
  double nu = params.get<double>("nu");

  // elemental information
  apf::DynamicMatrix Ke;
  apf::DynamicVector Ke_row;
  std::vector<LO> lids;

  // create the stiffness matrix integrator
  LHS = new ElasticStiffness(mesh, int_order, E, nu);

  // iterate over all elements in the mesh
  apf::MeshEntity* ent;
  auto it = mesh->begin(mesh->getDimension());
  while ((ent = mesh->iterate(it))) {

    // create a mesh element to pass to the integrator
    apf::MeshElement* me = apf::createMeshElement(mesh, ent);
    
    // integrate over the current element
    LHS->process( me);

    // get elemental stiffness information
    disc->get_ghost_lids( ent, lids);
    Ke = LHS->Ke;
    int num_rows = Ke.getRows();

    // add elemental stiffness rows into the full system
    for (int i = 0; i < num_rows; ++i)
    {
      LO row = lids[i];
      LHS->Ke.getRow(i, Ke_row);
      auto cols = Teuchos::arrayView(&(lids[0]), num_rows);
      auto values = Teuchos::arrayView(&(Ke_row[0]), num_rows);
      la.ghost->K->sumIntoLocalValues(row, cols, values);
    }

    // destroy the mesh element to prevent memory leaks
    apf::destroyMeshElement(me);
  }
  mesh->end(it);

  // destroy the stiffness matrix integrator
  delete LHS;
}

void FESolver::assemble_RHS()
{
  apply_nbcs( disc, &la, params);
  //TODO
  // apply_imgTractions();
  return;
}

void FESolver::solve()
{
  assemble_LHS();
  assemble_RHS();
  la.gather_K();
  la.gather_F();
  apply_dbcs( disc, &la, params);
  la.owned->K->fillComplete();
  auto solve_params = params.sublist("linear algebra");
  solve_linear_system( solve_params, &la, disc);
  return;
}

void FESolver::set_disp_to_field( apf::Field* f)
{
  set_to_field( f, la.owned->U, disc);
  return;
}

void FESolver::set_stress_to_field( apf::Field* f)
{
  double E = params.get<double>("E");
  set_Cauchy_stress( E, f, la.owned->U, disc);
  return;
}

}
