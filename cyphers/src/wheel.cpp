#include "wheel.hpp"

Wheel::Wheel()
{
  construct_alphabet();

  length = alphabet.size();

}

Wheel::~Wheel()
{

}

void Wheel::construct_alphabet()
{
  for( char letter='a'; letter <= 'z'; letter++)
  { 
    alphabet += letter;
  }

  for( int i=0; i < 10; i++)
  {
    alphabet += std::to_string( i);
  }

}

void Wheel::print_wheel()
{
  std::cout << "The wheel's alphabet is: " << std::endl;
  std::cout << alphabet << std::endl;
}

char Wheel::get_letter( char letter_in, str base, int i)
{
  int j = i % base.size();

  bool upper = std::isupper( letter_in);
  if( upper)
  {
    letter_in = std::tolower( letter_in);
  }

  int p = get_letter_pos( letter_in);
  int b = get_letter_pos( base[j]);

  int s = (p+b) % length;
  char letter_out = alphabet[ s];

  if( upper)
  {
    letter_out = std::toupper( letter_out);
  }

  return letter_out;
}

int Wheel::get_letter_pos( char letter_in)
{
  for( int i = 0; i < length; i++)
  {
    if( letter_in == alphabet[i])
    {
      return i;
    }
  }
  std::cout
    << "Failed to get alphabet position for letter: "
    << letter_in
    << std::endl;

  std::abort();
} 
