export EIG_SRC=/lore/clougj/Learning_Codes/FEA/Eigen_Package
export FEA_HW1_SRC=/lore/clougj/Learning_Codes/FEA/HW1/src
export FEA_HW5_SRC=/lore/clougj/Learning_Codes/FEA/HW5/src

cmake \
  -D EIG_DIR=${EIG_SRC} \
  -D CMAKE_CXX_FLAGS="-g -O2 -Wall -Werror" \
  -D HW1_SRC=${FEA_HW1_SRC} \
  -D HW5_SRC=${FEA_HW5_SRC} \
  ../
