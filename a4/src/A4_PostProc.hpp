#ifndef A4_POSTPROC_HPP 
#define A4_POSTPROC_HPP

/// @file A4_PostProc.hpp

#include <apf.h>
#include <apfShape.h>
#include <apfMesh.h>
#include <apfNumbering.h>
#include <apfDynamicMatrix.h>
#include "A4_Defines.hpp"
#include "A4_Disc.hpp"
#include <iostream>

namespace A4{

/// @brief Sets the values in RCP<vector> v to field f.
/// @param f The field to write to (using apf::setVector()).
/// @param v The vector of values.
/// @param d The discretization for node information.
void set_to_field( apf::Field* f, RCP<Vector> v, Disc* d);

/// @brief Calculates the Cauchy stress tensor and sets tensor to field.
/// @brief E Young's Modulus of the material.
/// @param f The field to write to (using apf::setMatrix()).
/// @param U The displacement solution vector.
/// @param d The relevant discretization object.
void set_Cauchy_stress( double E, apf::Field* f, RCP<Vector> U, Disc* d);


/// @brief Computes the L_2 norm of the error in the 
///        approximation of the solution U.
/// @param g The body load vector.
/// @param U The approximated solution vector.
/// @param d The discretization object.
/// @param E Young's modulus.
/// @param nu Poisson's ratio.
double get_L2_error( 
    double* g, 
    RCP<Vector> U, 
    Disc* d, 
    double E, 
    double nu);

} // End namespace A4
#endif
