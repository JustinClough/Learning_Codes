#include <cstdarg>
#include <cstdlib>
#include <PCU.h>
#include <Kokkos_Core.hpp>

#include "A4_Control.hpp"

namespace A4 {

static bool is_goal_initd = false;
static bool is_mpi_initd = false;
static bool is_kokkos_initd = false;
static bool is_pcu_initd = false;

static void call_mpi_init()
{
  MPI_Init(0, 0);
  is_mpi_initd = true;
}

static void call_pcu_init()
{
  PCU_Comm_Init();
  is_pcu_initd = true;
}

static void call_kokkos_init()
{
  Kokkos::initialize();
  is_kokkos_initd = true;
}

void initialize(bool init_mpi, bool init_kokkos, bool init_pcu)
{
  if (is_goal_initd) return;
  if (init_mpi) call_mpi_init();
  if (init_kokkos) call_kokkos_init();
  if (init_pcu) call_pcu_init();
  is_goal_initd = true;
}

static void call_mpi_free()
{
  //MPI_Finalize();
  is_mpi_initd = false;
}

static void call_kokkos_free()
{
  Kokkos::finalize();
  is_kokkos_initd = false;
}

static void call_pcu_free()
{
  PCU_Comm_Free();
  is_pcu_initd = false;
}

static void assert_initd()
{
  A4_DEBUG_ASSERT_VERBOSE(
      is_goal_initd,
      "goal::initialize() not called");
}

void finalize()
{
  assert_initd();
  if (is_pcu_initd) call_pcu_free();
  if (is_kokkos_initd) call_kokkos_free();
  if (is_mpi_initd) call_mpi_free();
  is_goal_initd = false;
}

void print(const char* message, ...)
{
  assert_initd();
  if (PCU_Comm_Self()) return void();
  va_list ap;
  va_start(ap, message);
  vfprintf(stdout, message, ap);
  va_end(ap);
  printf("\n");
}

void fail(const char* why, ...)
{
  assert_initd();
  va_list ap;
  va_start(ap, why);
  vfprintf(stderr, why, ap);
  va_end(ap);
  printf("\n");
  abort();
}

void assert_fail(const char* why, ...)
{
  assert_initd();
  fprintf(stderr, "%s", why);
  abort();
}

double time()
{
  assert_initd();
  return PCU_Time();
}

} // end namespace A4
