export EIG_SRC=/lore/clougj/Learning_Codes/FEA/Eigen_Package
export FEA_HW1_SRC=/lore/clougj/Learning_Codes/FEA/HW1/src
export FEA_HW5_SRC=/lore/clougj/Learning_Codes/FEA/HW5/src
export FEA_HW6_SRC=/lore/clougj/Learning_Codes/FEA/HW6/src

cmake \
  -D EIG_DIR=${EIG_SRC} \
  -D CMAKE_CXX_FLAGS="-g -O3 -Wall -Werror -lrt" \
  -D HW1_SRC=${FEA_HW1_SRC} \
  -D HW5_SRC=${FEA_HW5_SRC} \
  -D HW6_SRC=${FEA_HW6_SRC} \
  ../
