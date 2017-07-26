#include <apfMesh.h>
#include <apfShape.h>

#include "A4_ElasticStiffness.hpp"

namespace A4 {

static void fill_elast_tensor(apf::DynamicMatrix& D, double E, double v)
{
  // ASSUMES PLANE STRESS
  D.setSize(3, 3);
  D.zero();
  D(0,0) =  1;
  D(0,1) =  v;
  D(0,2) =  0;
  D(1,0) =  v;
  D(1,1) =  1;
  D(1,2) =  0;
  D(2,0) =  0;
  D(2,1) =  0;
  D(2,2) =  (1-v)/2;

  D*=(E/(1-v*v));
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

  B.setSize(3, num_elem_dofs);
  BT.setSize(num_elem_dofs, 3);
  DB.setSize(3, num_elem_dofs);
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
    B(0,2*i)     = dN[i][0];

    B(1,2*i+1)   = dN[i][1];

    B(2,2*i)     = dN[i][1];
    B(2,2*i+1)   = dN[i][0];
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
