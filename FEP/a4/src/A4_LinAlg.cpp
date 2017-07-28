#include "A4_Disc.hpp"
#include "A4_LinAlg.hpp"

namespace A4 {

LinAlg::LinAlg(Disc* d)
{
  owned = new LinAlgData;
  ghost = new LinAlgData;

  auto owned_map = d->get_owned_map();
  auto ghost_map = d->get_ghost_map();
  auto owned_graph = d->get_owned_graph();
  auto ghost_graph = d->get_ghost_graph();

  importer = rcp(new Import(owned_map, ghost_map));
  exporter = rcp(new Export(ghost_map, owned_map));

  owned->K = rcp(new Matrix(owned_graph));
  owned->U = rcp(new Vector(owned_map));
  owned->F = rcp(new Vector(owned_map));

  ghost->K = rcp(new Matrix(ghost_graph));
  ghost->U = rcp(new Vector(ghost_map));
  ghost->F = rcp(new Vector(ghost_map));
}

void LinAlg::gather_K()
{
  owned->K->doExport(*(ghost->K), *exporter, Tpetra::ADD);
}

void LinAlg::gather_F()
{
  owned->F->doExport(*(ghost->F), *exporter, Tpetra::ADD);
}

LinAlg::~LinAlg()
{
  delete ghost;
  delete owned;
}

LinAlg* create_lin_alg(Disc* d)
{
  return new LinAlg(d);
}

void destroy_lin_alg(LinAlg* l)
{
  delete l;
}

} // end namespace A4
