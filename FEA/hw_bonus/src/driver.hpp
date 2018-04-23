#ifndef DRIVER_HPP
#define DRIVER_HPP

// Checks the argument inputs
void check_inputs( int     argc, 
                   char**  argv,
                   int*    pnp1,
                   int*    pmeth);

// Drives the problem
void drive_problem( int Np1, int method);

#endif
