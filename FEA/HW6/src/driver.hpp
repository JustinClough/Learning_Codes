#ifndef DRIVER_HPP
#define DRIVER_HPP

// Checks the argument inputs
void check_inputs( int     argc, 
                   char**  argv,
                   int*    pcn,
                   int*    pnp1,
                   int*    pmeth,
                   double* pdt   );

// Drives the problem
void drive_problem( int CaseNumber, int Np1);

#endif
