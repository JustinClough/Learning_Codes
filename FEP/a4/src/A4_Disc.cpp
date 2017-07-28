#include <apf.h>
#include <apf.h>
#include <apfAlbany.h>
#include <apfMDS.h>
#include <apfMesh2.h>
#include <apfShape.h>
#include <gmi_mesh.h>

#include "A4_Disc.hpp"
#include "A4_Control.hpp"

namespace A4 {

static apf::StkModels* read_sets(apf::Mesh* m, const char* filename)
{
  auto sets = new apf::StkModels;
  print("reading association file: %s", filename);
  static std::string const setNames[3] = {
      "node set", "side set", "elem set"};
  auto d = m->getDimension();
  int dims[3] = {0, d - 1, d};
  std::ifstream f(filename);
  if (!f.good()) fail("cannot open file: %s", filename);
  std::string sline;
  int lc = 0;
  while (std::getline(f, sline)) {
    if (!sline.length()) break;
    ++lc;
    int sdi = -1;
    for (int di = 0; di < 3; ++di)
      if (sline.compare(0, setNames[di].length(), setNames[di]) == 0) sdi = di;
    if (sdi == -1)
      fail("invalid association line # %d:\n\t%s", lc, sline.c_str());
    int sd = dims[sdi];
    std::stringstream strs(sline.substr(setNames[sdi].length()));
    auto set = new apf::StkModel();
    strs >> set->stkName;
    int nents;
    strs >> nents;
    if (!strs) fail("invalid association line # %d:\n\t%s", lc, sline.c_str());
    for (int ei = 0; ei < nents; ++ei) {
      std::string eline;
      std::getline(f, eline);
      if (!f || !eline.length())
        fail("invalid association after line # %d", lc);
      ++lc;
      std::stringstream strs2(eline);
      int mdim, mtag;
      strs2 >> mdim >> mtag;
      if (!strs2) fail("bad associations line # %d:\n\t%s", lc, eline.c_str());
      set->ents.push_back(m->findModelEntity(mdim, mtag));
      if (!set->ents.back())
        fail("no model entity with dim: %d and tag: %d", mdim, mtag);
    }
    sets->models[sd].push_back(set);
  }
  sets->computeInverse();
  return sets;
}

Disc::Disc(apf::Mesh* m, const char* assoc)
    : mesh(m),
      shape(m->getShape()),
      owned_nmbr(0),
      ghost_nmbr(0),
      global_nmbr(0)
{
  auto t0 = time();
  mesh->verify();
  sets = read_sets(m, assoc);
  num_eqs = mesh->getDimension();
  num_dims = mesh->getDimension();
  comm = Tpetra::DefaultPlatform::getDefaultPlatform().getComm();
  compute_owned_map();
  compute_ghost_map();
  compute_graphs();
  compute_coords();
  compute_side_sets();
  compute_node_sets();
  auto t1 = time();
  print(" num side sets: %d", get_num_side_sets());
  print(" num node sets: %d", get_num_node_sets());
  print(" disc built in %f seconds", t1 - t0);
}

Disc::~Disc()
{
  if (sets) delete sets;
  comm = Teuchos::null;
  owned_map = Teuchos::null;
  owned_map = Teuchos::null;
  ghost_map = Teuchos::null;
  node_map = Teuchos::null;
  coords = Teuchos::null;
  owned_graph = Teuchos::null;
  ghost_graph = Teuchos::null;
}

static LO get_dof(const LO nid, const int eq, const int eqs)
{
  return nid * eqs + eq;
}

static GO get_gdof(const GO nid, const int eq, const int eqs)
{
  return nid * eqs + eq;
}

int Disc::get_num_dofs(apf::MeshEntity* e) {
  auto type = mesh->getType(e);
  auto es = shape->getEntityShape(type);
  auto num_nodes = es->countNodes();
  return num_nodes * num_eqs;
}

void Disc::get_ghost_lids(apf::MeshEntity* e, std::vector<LO>& lids) {
  lids.resize(get_num_dofs(e));
  static apf::NewArray<int> node_ids;
  int num_nodes = apf::getElementNumbers(ghost_nmbr, e, node_ids);
  int dof = 0;
  for (int node = 0; node < num_nodes; ++node)
    for (int eq = 0; eq < num_eqs; ++eq)
      lids[dof++] = get_dof(node_ids[node], eq, num_eqs);
}

LO Disc::get_owned_lid(apf::Node const& n, int eq)
{
  LO nid = apf::getNumber(owned_nmbr, n.entity, n.node, 0);
  return get_dof(nid, eq, num_eqs);
}

int Disc::get_num_side_sets() const
{
  return sets->models[num_dims - 1].size();
}

int Disc::get_num_node_sets() const
{
  return sets->models[0].size();
}

std::string Disc::get_side_set_name(const int i) const
{
  A4_DEBUG_ASSERT(i < get_num_side_sets());
  return sets->models[num_dims - 1][i]->stkName;
}

std::string Disc::get_node_set_name(const int i) const
{
  A4_DEBUG_ASSERT(i < get_num_node_sets());
  return sets->models[0][i]->stkName;
}

std::vector<apf::MeshEntity*> const& Disc::get_sides(
    std::string const& side_set)
{
  if (! side_sets.count(side_set))
    fail("side set %s not found", side_set.c_str());
  return side_sets[side_set];
}

std::vector<apf::Node> const& Disc::get_nodes(
    std::string const& node_set)
{
  if (! node_sets.count(node_set))
    fail("node set %s not found", node_set.c_str());
  return node_sets[node_set];
}

void Disc::compute_owned_map()
{
  A4_DEBUG_ASSERT(! owned_nmbr);
  A4_DEBUG_ASSERT(! global_nmbr);
  owned_nmbr = apf::numberOwnedNodes(mesh, "owned", shape);
  global_nmbr = apf::makeGlobal(owned_nmbr, false);
  apf::getNodes(global_nmbr, owned);
  auto num_owned = owned.getSize();
  Teuchos::Array<GO> indices(num_owned);
  for (size_t n = 0; n < num_owned; ++n)
    indices[n] = apf::getNumber(global_nmbr, owned[n]);
  node_map = Tpetra::createNonContigMap<LO, GO>(indices, comm);
  indices.resize(num_eqs * num_owned);
  for (size_t n = 0; n < num_owned; ++n)
  {
    GO gid = apf::getNumber(global_nmbr, owned[n]);
    for (int eq = 0; eq < num_eqs; ++eq)
      indices[get_dof(n, eq, num_eqs)] = get_gdof(gid, eq, num_eqs);
  }
  owned_map = Tpetra::createNonContigMap<LO, GO>(indices, comm);
  apf::synchronize(global_nmbr);
}

void Disc::compute_ghost_map()
{
  A4_DEBUG_ASSERT(! ghost_nmbr);
  ghost_nmbr = apf::numberOverlapNodes(mesh, "ghost", shape);
  apf::DynamicArray<apf::Node> ghost;
  apf::getNodes(ghost_nmbr, ghost);
  auto num_ghost = ghost.getSize();
  Teuchos::Array<GO> indices(num_eqs * num_ghost);
  for (size_t n = 0; n < num_ghost; ++n)
  {
    GO gid = apf::getNumber(global_nmbr, ghost[n]);
    for (int eq = 0; eq < num_eqs; ++eq)
      indices[get_dof(n, eq, num_eqs)] = get_gdof(gid, eq, num_eqs);
  }
  ghost_map = Tpetra::createNonContigMap<LO, GO>(indices, comm);
}

void Disc::compute_graphs()
{
  owned_graph = rcp(new Graph(owned_map, 300));
  ghost_graph = rcp(new Graph(ghost_map, 300));
  apf::MeshEntity* elem;
  auto elems = mesh->begin(num_dims);
  while ((elem = mesh->iterate(elems)))
  {
    apf::NewArray<long> gids;
    int nnodes = apf::getElementNumbers(global_nmbr, elem, gids);
    for (int i = 0; i < nnodes; ++i)
    {
      for (int j = 0; j < num_eqs; ++j)
      {
        GO row = get_gdof(gids[i], j, num_eqs);
        for (int k = 0; k < nnodes; ++k)
        {
          for (int l = 0; l < num_eqs; ++l)
          {
            GO col = get_gdof(gids[k], l, num_eqs);
            auto col_av = Teuchos::arrayView(&col, 1);
            ghost_graph->insertGlobalIndices(row, col_av);
          }
        }
      }
    }
  }
  mesh->end(elems);
  ghost_graph->fillComplete();
  auto exporter = rcp(new Export(ghost_map, owned_map));
  owned_graph->doExport(*ghost_graph, *exporter, Tpetra::INSERT);
  owned_graph->fillComplete();
  apf::destroyGlobalNumbering(global_nmbr);
}

void Disc::compute_coords()
{
  coords = rcp(new MultiVector(node_map, num_dims, false));
  apf::Vector3 point(0, 0, 0);
  for (size_t n = 0; n < owned.size(); ++n)
  {
    mesh->getPoint(owned[n].entity, owned[n].node, point);
    for (int d = 0; d < num_dims; ++d)
      coords->replaceLocalValue(n, d, point[d]);
  }
}

void Disc::compute_side_sets()
{
  int nss = sets->models[num_dims-1].size();
  for (int i = 0; i < nss; ++i)
    side_sets[get_side_set_name(i)].resize(0);
  apf::MeshIterator* it = mesh->begin(num_dims-1);
  apf::MeshEntity* side;
  while ((side = mesh->iterate(it)))
  {
    apf::ModelEntity* me = mesh->toModel(side);
    if (!sets->invMaps[num_dims-1].count(me))
      continue;
    apf::StkModel* fs = sets->invMaps[num_dims-1][me];
    std::string const& fsn = fs->stkName;
    apf::Up adjElems;
    mesh->getUp(side, adjElems);
    A4_DEBUG_ASSERT(adjElems.n == 1);
    side_sets[fsn].push_back(side);
  }
  mesh->end(it);
}

void Disc::compute_node_sets()
{
  auto nns = sets->models[0].size();
  for (size_t s = 0; s < nns; ++s)
    node_sets[get_node_set_name(s)].resize(0);
  for (size_t n = 0; n < owned.size(); ++n)
  {
    auto node = owned[n];
    auto ent = node.entity;
    std::set<apf::StkModel*> mset;
    apf::collectEntityModels(
        mesh, sets->invMaps[0], mesh->toModel(ent), mset);
    if (mset.empty()) continue;
    APF_ITERATE(std::set<apf::StkModel*>, mset, mit)
    {
      auto ns = *mit;
      auto nsn = ns->stkName;
      node_sets[nsn].push_back(node);
    }
  }
}

} // end namespace A4
