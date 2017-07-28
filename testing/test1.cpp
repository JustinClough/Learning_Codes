#include <iostream>

int main()
{
  int i = 1;
  bool T = true;
  while((i++) && T)
  {
    std::cout << "i = " << i << std::endl;
    T = false;
  }
  std::cout << "i= " << i << std::endl;

  return 0;
}
