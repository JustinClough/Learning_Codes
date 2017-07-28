#!/bin/bash

A4_DIR=/lore/clougj/FiniteElementProgramming/a4

# Load appropriate modules
source $A4_DIR/env-scorec.sh

# Move to main project directory
cd $A4_DIR

# Delete old build diretory if one exists
if [ -d "$A4_DIR/build" ]; then
  rm -r $A4_DIR/build/
fi

# Make new build dirtory, move there
mkdir $A4_DIR/build/
cd $A4_DIR/build/

# Buid the project via cmake
cmake \
-DCMAKE_C_COMPILER=mpicc \
-DCMAKE_CXX_COMPILER=mpicxx \
-DCMAKE_INSTALL_PREFIX=../install \
-DSCOREC_PREFIX=/usr/local/trilinos/latest \
..

# Move back to main project directory
cd $A4_DIR
