#include "input_handler.hpp"


bool parse_inputs( int argc, char** argv, Cypher* cyph)
{
  if( argc != 4)
  {
    return false;
  }

  cyph->set_base( argv[1]);

  return true;
}
