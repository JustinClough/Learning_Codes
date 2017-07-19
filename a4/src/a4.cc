#include "A4_Disc.hpp"
#include "A4_Control.hpp"
#include "A4_Util.hpp"

// APF, GMI Headers
#include <apfMDS.h>
#include <apfMesh2.h>
#include <apf.h>
#include <apfShape.h>
#include <gmi_mesh.h>

int main( int argc, char** argv)
{
  A4::initialize(true, true, true);

  Teuchos::ParameterList p;
  auto pp = Teuchos::Ptr<Teuchos::ParameterList> (&p);

  // load the mesh and create a discretization
  A4_ALWAYS_ASSERT(argc == 5);
  A4::print("USING MODEL: %s", argv[1]);
  A4::print("USING MESH: %s", argv[2]);
  A4::print("USING ASSOC: %s", argv[3]);
  gmi_register_mesh();

  Teuchos::updateParametersFromYamlFile(argv[4], pp);

  apf::Mesh2* m = apf::loadMdsMesh(argv[1], argv[2]);
  auto disc = new A4::Disc(m, argv[3]);

  auto solver = new A4::FESolver(disc, *pp);
  solver->solve();

  char name[] = "Displacement";
  apf::Field* disp = apf::createFieldOn( m, name, apf::VECTOR);
  apf::zeroField( disp);
  solver->set_disp_to_field( disp);

  char stress_name[] = "Cauchy Stress";
  //apf::Field* stress_f = apf::createFieldOn( m, stress_name, apf::MATRIX);
  apf::Field* stress_f = apf::createIPField( 
      m, stress_name, apf::MATRIX, (m->getShape())->getOrder());
  apf::zeroField( stress_f);
  solver->set_stress_to_field( stress_f);

  char fileName[] = "FE_Solution";
  apf::writeVtkFiles( fileName, m, 3);
  apf::destroyField( disp);
  apf::destroyField( stress_f);

  // clean up the junk we created
  delete solver;
  delete disc;

  // destroy the mesh
  m->destroyNative();
  apf::destroyMesh(m);

  A4::finalize();

  return 0;
}
