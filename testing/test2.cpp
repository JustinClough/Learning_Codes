#include <iostream>

int main () 
{
  bool a;
  if (a)
  {
    std::cout << "Default is true" << std::endl;
  }
  else if (a==false)
  {
    std::cout << "Default is false" << std::endl;
  }
  else
  {
    std::cout << "what" << std::endl;
  }
  return 0;
}
