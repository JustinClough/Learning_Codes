export EIG_SRC=/lore/clougj/Learning_Codes/FEA/Eigen_Package
export FEA_HW1_SRC=/lore/clougj/Learning_Codes/FEA/HW1/src

cmake \
  -D EIG_DIR=${EIG_SRC} \
  -D HW1_SRC=${FEA_HW1_SRC} \
  ../
