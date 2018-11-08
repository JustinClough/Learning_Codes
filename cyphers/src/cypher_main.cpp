#include "input_handler.hpp"
#include "cypher_class.hpp"

int main( int argc, char** argv)
{
  Cypher* cyph = new Cypher();
  if( !parse_inputs( argc, argv, cyph))
  {
    std::cout 
    << "Usage:" << std::endl
    << argv[0] << " base target.txt output.txt"
    << std::endl;
  
    std::abort();
  }

  cyph->operate();
  delete cyph;
  return 0;
}
