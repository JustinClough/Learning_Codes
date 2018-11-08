#include "input_handler.hpp"

bool parse_inputs( int argc, char** argv, Cypher* cyph)
{
  if( argc != 4)
  {
    return false;
  }

  cyph->set_base( argv[1]);
  cyph->set_target( argv[2]);
  cyph->set_result( argv[3]);

  return true;
}
