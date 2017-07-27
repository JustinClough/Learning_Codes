#include "A4_Disc.hpp"
#include "A4_Control.hpp"
#include "A4_FESolver.hpp"

// Trilinos Headers
#include <Teuchos_YamlParameterListHelpers.hpp>

// APF, GMI Headers
#include <apfMDS.h>
#include <apfMesh2.h>
#include <apf.h>
#include <apfShape.h>
#include <gmi_mesh.h>

#include <stdlib.h>

int main( int argc, char** argv)
{
  A4::initialize(true, true, true);

  Teuchos::ParameterList p;
  auto pp = Teuchos::Ptr<Teuchos::ParameterList> (&p);

  // Get info from argv's
  A4_ALWAYS_ASSERT(argc == 10);
  A4::print("USING MODEL: %s", argv[1]);
  A4::print("USING MESH: %s", argv[2]);
  A4::print("USING ASSOC: %s", argv[3]);
  A4::print("SOLVING: %s", argv[5]);
  A4::print("INTEGRATION ORDER: %s", argv[6]);
  A4::print("BODY LOAD: %s, %s, %s", argv[7], argv[8], argv[9]);
  double bodyLoad[3] = {
    std::atof( argv[7]),
    std::atof( argv[8]),
    std::atof( argv[9])};
  int order = std::atoi( argv[6]);
  Teuchos::updateParametersFromYamlFile(argv[4], pp);

  // Load mesh and create discretization object
  gmi_register_mesh();
  apf::Mesh2* m = apf::loadMdsMesh(argv[1], argv[2]);

  if( order == 1)
  {
    // No changes
  }
  else if( order == 2)
  {
    auto shape = apf::getSerendipity();
    m->changeShape( shape);
  }
  else
  {
    std::cout << "Order" << order << " not supported" << std::endl;
    std::abort();
  }

  auto disc = new A4::Disc(m, argv[3]);

  // Create linear algebra solver and solve system
  auto solver = new A4::FESolver(disc, *pp, order, bodyLoad);
  solver->solve();

  // Write out solution, force, and stress to fields as vtk files
  char disp_name[] = "Displacement";
  apf::Field* disp = apf::createFieldOn( m, disp_name, apf::VECTOR);
  apf::zeroField( disp);
  solver->set_disp_to_field( disp);

  char force_name[] = "Traction";
  apf::Field* force = apf::createFieldOn( m, force_name, apf::VECTOR);
  apf::zeroField( force);
  solver->set_force_to_field( force);

  char stress_name[] = "Cauchy Stress";
  apf::Field* stress_f = apf::createIPField( 
      m, stress_name, apf::MATRIX, (m->getShape())->getOrder());
  apf::zeroField( stress_f);
  solver->set_stress_to_field( stress_f);

  // Write solution
  char* fileName = argv[5];
  apf::writeVtkFiles( fileName, m, m->getDimension());

  std::cout << "APPROXIMATION ERROR IS: " << solver->get_error() << std::endl;
  
  // delete fields
  apf::destroyField( disp);
  apf::destroyField( force);
  apf::destroyField( stress_f);

  // delete discretization and solver objects
  delete solver;
  delete disc;

  // destroy the mesh
  m->destroyNative();
  apf::destroyMesh(m);

  // close mpi services
  A4::finalize();

  return 0;
}
