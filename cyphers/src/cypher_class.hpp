#ifndef CYPHER_CLASS_HPP
#define CYPHER_CLASS_HPP

#include <string>

using str = std::string;

class Cypher
{
  public:
    Cypher();
    ~Cypher();

    void set_base( char* base);

  private:
    str base;
  


};

#endif
