#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "eig_wrap.hpp"
#include "mesh1D.hpp"
#include "stiffness.hpp"
#include "forcing.hpp"
#include "errorCalcs.hpp"

void drive_problem( double oddSize, 
                    double evenSize,
                    int*   NplusOneArray,
                    int    numTests,
                    int    caseNumber);

#endif
