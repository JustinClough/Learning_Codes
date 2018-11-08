#ifndef CYPHER_CLASS_HPP
#define CYPHER_CLASS_HPP

#include <string>
#include <cerrno>
#include <fstream>
#include <iostream>
#include "wheel.hpp"

using str = std::string;

class Cypher
{
  public:
    Cypher();
    ~Cypher();

    void set_base( char* base);
    void set_target( char* target);
    void set_result( char* result);

    void print_status();

    void operate();

  private:
    Wheel* wheel;
    str base; 
    str target;
    str result;

    str get_file_contents( const char* filename);
    str encrypt( str pt);
    bool encryptable( char letter);
    void write_to_file( str enc);

    str get_safe_letters();

};

#endif
