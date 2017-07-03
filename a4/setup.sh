#!/bin/bash -e
set -x
[ ! -e build ] && mkdir build
cd build
flags='-g '
cmake -DCMAKE_CXX_COMPILER=mpicxx -DCMAKE_CXX_FLAGS=$flags -DSCOREC_PREFIX=/usr/local/pumi/core/lib/cmake/SCOREC ../
cd -
set +x
