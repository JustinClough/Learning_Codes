#include <iostream>
#include <cmath>

int main()
{

  char c = 'c';
  char* pc = &c;
  
  double  a = nan( pc);
  double* pa = &a;
  std::cout << "pa[0] = " << pa[0] << std::endl;

  double  b = 10.0;
  double* pb = &b;
  std::cout << "pb[0] = " << pb[0] << std::endl;

  if ( pa[0] != pa[0])
  {
    std::cout << " pa[0] != pa[0] " << std::endl;
  }

return 0;
}
