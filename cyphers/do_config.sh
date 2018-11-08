export CYPHER_HOME=/home/jlclough/Learning_Codes/cyphers
export CYPHER_SRC=${CYPHER_HOME}/src
export CYPHER_BLD=${CYPHER_HOME}/build

cmake \
  -D CYPHER_SRC=${CYPHER_SRC} \
  -D CMAKE_CXX_FLAGS="-O3 -Wall -Werror -lrt" \
  ../
