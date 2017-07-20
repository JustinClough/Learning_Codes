#include <A4_BodyLoads.hpp>

namespace A4
{

using Teuchos::getValue;
using Teuchos::Array;

void apply_body_loads(
  Disc* d,
  LinAlg* la,
  int order,
  double load[3])
{
  // Get info from inputs
  apf::Vector3 bodyLoad ( load[0], load[1], load[2]);
  auto mesh = d->get_apf_mesh();

  auto blInter = new BodyLoad( mesh, order, bodyLoad);

  // Iterate over all highest dimension mesh elements
  apf::MeshEntity* ent;
  auto it = mesh->begin( mesh->getDimension());
  while( (ent = mesh->iterate(it)) )
  {
    // Get local ids of the dofs of this entity
    std::vector<LO> lids;
    d->get_ghost_lids( ent, lids);

    apf::MeshElement* elem = apf::createMeshElement( mesh, ent);

    // Integrate over this element
    blInter->process( elem);
    auto fe = blInter->get_fe();

    // Sum into global forcing vector
    for( size_t i = 0; i < fe.size(); i++)
    {
      LO row = lids[i];
      auto f = fe[i];
      la->ghost->F->sumIntoLocalValue( row, f);
    }

    apf::destroyMeshElement( elem);
  }
  mesh->end( it);

  delete blInter;

  return;
}

BodyLoad::BodyLoad(
    apf::Mesh* mesh,
    int inte_order,
    apf::Vector3 load):
  apf::Integrator( inte_order)
{
  m = mesh;
  order = inte_order;
  shape = m->getShape();
  g = load;
  return;
}

void BodyLoad::inElement( 
    apf::MeshElement* me)
{
  elem = me;
  auto ent = apf::getMeshEntity( elem);
  auto type = m->getType( ent);
  auto es = shape->getEntityShape( type);
  num_dims = m->getDimension();
  num_elem_nodes = es->countNodes();
  num_elem_dofs = num_dims * num_elem_nodes;
  
  fe.setSize( num_elem_dofs);
  fe.zero();
  
  return;
}

void BodyLoad::atPoint(
        apf::Vector3 const& p,
        double w, 
        double dv)
{
  apf::getBF( shape, elem, p, N);
  apf::DynamicVector f_tmp (num_elem_dofs);
  f_tmp.zero();
  int ind = 0;

  for( int i =0; i < num_elem_nodes; ++i)
  {
    for( int j = 0; j < num_dims; ++j)
    {
      ind = i*num_dims+j;
      f_tmp[ind] += N[i]*g[i]*w*dv;
    }
  }

  fe += f_tmp;

  return;
}

void BodyLoad::outElement()
{
  elem = 0;
  return;
}

} // End namespace A4
