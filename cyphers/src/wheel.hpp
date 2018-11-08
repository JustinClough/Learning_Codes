#ifndef WHEEL_HPP
#define WHEEL_HPP

#include <iostream>
#include <cctype>
#include <cstdlib>

using str = std::string;

class Wheel
{
  public:
    Wheel();
    ~Wheel();


    char get_letter( char letter_in, str base, int i);

    void print_wheel();

  private:
    str alphabet;

    int length;
    void construct_alphabet();

    int get_letter_pos( char letter_in);
};

#endif
