#include "A4_PostProc.hpp"

namespace A4{

void set_to_field( apf::Field* f, RCP<Vector> v, Disc* d)
{
  auto u = v->get1dView();
  auto owned_nmbr = d->get_owned_numbering();
  apf::Vector3 value;

  apf::DynamicArray<apf::Node> nodes;
  apf::getNodes( owned_nmbr, nodes);
  for( size_t n = 0; n < nodes.size(); n++)
  {
    auto node = nodes[n];
    auto e = node.entity;
    auto nodeth = node.node;
    for (int i = 0; i < 3; i++)
    {
      auto row = d->get_owned_lid( node, i);
      value[i] = u[row];
    } 
    apf::setVector( f, e, nodeth, value);
  }
  apf::synchronize( f);
  return;
}

void get_elemental_solution( 
    apf::DynamicVector& u_e, 
    RCP<Vector> U, 
    std::vector<LO> lids)
{
  auto u = U->get1dView();
  
  for( size_t i = 0; i < lids.size(); i++)
  {
    auto row = lids[i];
    u_e[i] = u[row];
  }
  return;
}

void nye_to_matrix_3x3(
    apf::DynamicVector& v,
    apf::Matrix3x3& m)
{
  // Diagonal
  m[0][0] = v(0);
  m[1][1] = v(1);
  m[2][2] = v(2);

  // Upper tri
  m[1][2] = v(3);
  m[0][2] = v(4);
  m[0][1] = v(5);

  // Lower tri
  m[2][1] = v(3);
  m[2][0] = v(4);
  m[1][0] = v(5);

  return;
}

void set_elemental_stress(
    apf::MeshElement* elem,
    Disc* d,
    apf::Field* f,
    RCP<Vector> U,
    double E)
{
  auto ent = apf::getMeshEntity( elem);
  auto mesh = d->get_apf_mesh();
  auto shape = mesh->getShape();
  auto inte_order = shape->getOrder();

  // Get element's node ID's
  std::vector<LO> lids;
  d->get_ghost_lids( ent, lids);

  apf::Vector3 para;
  apf::NewArray<apf::Vector3> dN;
  apf::DynamicMatrix B;
  B.setSize( 6, lids.size());
  B.zero();

  apf::DynamicVector u_e (lids.size());
  u_e.zero();
  get_elemental_solution( u_e, U, lids);
  apf::DynamicVector s_e (6);


  int num_ips = apf::countIntPoints( elem, inte_order);
  for( int i = 0; i < num_ips; i++)
  {
    s_e.zero();
    apf::getIntPoint( elem, inte_order, i, para);
    apf::getGradBF(shape, elem, para, dN);

    size_t num_nodes = shape->getEntityShape(mesh->getType(ent))->countNodes();
    for( size_t j = 0; j < num_nodes; ++j)
    {
      B(0,3*j)   = dN[j][0];

      B(1,3*j+1) = dN[j][1];

      B(2,3*j+2) = dN[j][2];

      B(3,3*j)   = dN[j][1];
      B(3,3*j+1) = dN[j][0];

      B(4,3*j+1) = dN[j][2];
      B(4,3*j+2) = dN[j][1];

      B(5,3*j)   = dN[j][2];
      B(5,3*j+2) = dN[j][0];
    }
    apf::multiply( B, u_e, s_e);
    s_e *= E;
    apf::Matrix3x3 sigma;
    nye_to_matrix_3x3( s_e, sigma);
    apf::setMatrix( f, ent, i, sigma);
  }

  return;
}

void set_Cauchy_stress( double E, apf::Field* f, RCP<Vector> U, Disc* d)
{
  auto mesh = d->get_apf_mesh();

  // Iterate over each mesh region
  apf::MeshEntity* ent;
  apf::MeshIterator* ent_it = mesh->begin(mesh->getDimension());
  while ((ent = mesh->iterate(ent_it)))
  {
    auto elem = apf::createMeshElement( mesh, ent);
    set_elemental_stress( elem, d, f, U, E);
    apf::destroyMeshElement( elem);
  }
  mesh->end( ent_it);

  apf::synchronize( f);
  return;
}

} // End namespace A4
