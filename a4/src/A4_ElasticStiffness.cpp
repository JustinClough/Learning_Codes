#include <apfMesh.h>
#include <apfShape.h>

#include "A4_ElasticStiffness.hpp"

namespace A4 {

static void fill_elast_tensor(apf::DynamicMatrix& D, double E, double v)
{
  D.setSize(6, 6);
  D.zero();
  double lambda = ( v * E ) / ( ( 1 + v ) * ( 1 - 2 * v ) );
  double mu = E / ( 2 * ( 1 + v ) );
  D(0,0) = lambda + (2 * mu);
  D(0,1) = lambda;
  D(0,2) = lambda;
  D(1,0) = lambda;
  D(1,1) = lambda + (2 * mu);
  D(1,2) = lambda;
  D(2,0) = lambda;
  D(2,1) = lambda;
  D(2,2) = lambda + (2 * mu);
  D(3,3) = mu;
  D(4,4) = mu;
  D(5,5) = mu;
}

ElasticStiffness::ElasticStiffness(apf::Mesh* m, int o, double E, double nu)
    : apf::Integrator(o),
      num_elem_dofs(0),
      mesh(m),
      shape(0),
      mesh_element(0)
{
  num_dims = m->getDimension();
  shape = m->getShape();
  fill_elast_tensor(D, E, nu);
}

void ElasticStiffness::inElement(apf::MeshElement* me)
{
  mesh_element = me;

  auto ent = apf::getMeshEntity(mesh_element);
  auto type = mesh->getType(ent);
  auto es = shape->getEntityShape(type);
  num_elem_nodes = es->countNodes();
  num_elem_dofs = num_dims * num_elem_nodes;

  B.setSize(6, num_elem_dofs);
  BT.setSize(num_elem_dofs, 6);
  DB.setSize(6, num_elem_dofs);
  Ke.setSize(num_elem_dofs, num_elem_dofs);
  K_tmp.setSize(num_elem_dofs, num_elem_dofs);

  B.zero();
  Ke.zero();
  K_tmp.zero();
}

void ElasticStiffness::atPoint(apf::Vector3 const& p, double w, double dv)
{
  apf::getGradBF(shape, mesh_element, p, dN);

  for (int i = 0; i < num_elem_nodes; ++i)
  {
    B(0,3*i)   = dN[i][0];

    B(1,3*i+1) = dN[i][1];

    B(2,3*i+2) = dN[i][2];

    B(3,3*i)   = dN[i][1];
    B(3,3*i+1) = dN[i][0];

    B(4,3*i+1) = dN[i][2];
    B(4,3*i+2) = dN[i][1];

    B(5,3*i)   = dN[i][2];
    B(5,3*i+2) = dN[i][0];
  }

  apf::transpose(B, BT);
  apf::multiply(D, B, DB);
  apf::multiply(BT, DB, K_tmp);

  K_tmp *= w * dv;
  Ke += K_tmp;

}

void ElasticStiffness::outElement()
{
  mesh_element = 0;
}

} // end namespace A4
