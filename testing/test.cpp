#include <iostream>

int main( int argc, char** argv ) {
  int b = 5;
  int* a = &b;

  for(int i=0; i<*a; i++)
  {
    std::cout << "i = " << i << std::endl;
  }

  return 0;
}
