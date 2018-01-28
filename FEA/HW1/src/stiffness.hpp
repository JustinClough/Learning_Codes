#ifndef STIFFNESS_HPP
#define STIFFNESS_HPP

#include "mesh1D.hpp"
#include "eig_wrap.hpp"

MatrixXd create_stiffness( mesh1D* mesh, int caseNumber);

#endif
