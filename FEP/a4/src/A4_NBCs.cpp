#include "A4_NBCs.hpp"
#include "A4_Disc.hpp"
#include "A4_LinAlg.hpp"

#include <apfDynamicVector.h>
#include <apfShape.h>

#include <Teuchos_ParameterList.hpp>
#include <string>

namespace A4 {

using Teuchos::getValue;
using Teuchos::Array;

elemTrac::elemTrac(
      Disc* d,
      int order,
      double val,
      int eqNumber):
  apf::Integrator( order)
{
  value = val;
  eqn = eqNumber;
  mesh = d->get_apf_mesh();
  shape = mesh->getShape();
  num_dims = mesh->getDimension();
}

void elemTrac::inElement( apf::MeshElement* element)
{
  elem = element;
  auto ent = apf::getMeshEntity( element);
  auto type = mesh->getType( ent);
  auto es = shape->getEntityShape( type);
  num_elem_nodes = es->countNodes();
  num_elem_dofs = num_dims * num_elem_nodes;

  fe.setSize(num_elem_dofs);
  fe.zero();

  return;
}

void elemTrac::atPoint( apf::Vector3 const& p, double w, double dv)
{
  apf::NewArray<double> Ns;
  apf::getBF( shape, elem, p, Ns);
  apf::DynamicVector f_tmp (num_elem_dofs);
  f_tmp.zero();
  int ind = 0;

  for( int i =0; i < num_elem_nodes; ++i)
  {
    for( int j = 0; j < num_dims; ++j)
    {
      if( j == eqn)
      {
        ind = i*num_dims+j;
        f_tmp[ind] += Ns[i]*value*w*dv;
      }
    }
  }

  fe += f_tmp;
  return;
}

void elemTrac::outElement()
{
  elem = 0;
  return;
}


void apply_nbcs(
  Disc* d,
  LinAlg* la,
  ParameterList const& p,
  int inte_order)
{
  std::vector<LO> lids;
  auto nbcs = p.sublist( "traction bcs");
  auto mesh = d->get_apf_mesh();

  for( auto it = nbcs.begin(); it != nbcs.end(); ++it)
  {
    auto entry = nbcs.entry(it);
    // Format is: {0, xmin, 0.0} (spatial_dof, side_set_name, value)
    auto info = getValue<Array<std::string> >( entry);
    int s_dof = std::stoi( info[0]);
    std::string sideSet = info[1];
    double value = std::stod( info[2]);

    auto tracInter = new elemTrac( d, inte_order, value, s_dof);
    auto faces = d->get_sides( sideSet);
    for( int i = 0; i< (int)faces.size(); ++i)
    {
      apf::MeshEntity* face = faces[i];
      apf::MeshElement* elem = apf::createMeshElement( mesh, face);

      tracInter->process(elem);

      d->get_ghost_lids(face, lids);
      auto fe = tracInter->get_fe();
      int numRows = fe.getSize();
      for(int i = 0; i < numRows; i++)
      {
        auto row = lids[i];
        auto t = fe[i];
        la->ghost->F->sumIntoLocalValue( row, t);
      }
      apf::destroyMeshElement( elem);
    }
    delete tracInter;
  }

  return;
}

}
