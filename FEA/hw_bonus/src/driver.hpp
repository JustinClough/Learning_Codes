#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "printer.hpp"

// Checks the argument inputs
void check_inputs( int     argc, 
                   char**  argv,
                   int*    pmeth);

// Drives the problem
void drive_problem( int method);

void drive( int Np1, int method, Printer* printer);

#endif
