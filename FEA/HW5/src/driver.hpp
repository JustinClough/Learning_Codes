#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "eig_wrap.hpp"

void seed_random();

void drive_problem( int CaseNumber, int Np1);

void check_inputs( int argc, char** argv, int* pcn, int* pnp1);

#endif
