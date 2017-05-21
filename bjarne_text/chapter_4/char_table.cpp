#include <iostream>

using std::cout;
using std::endl;

int main()
{
  int i=0;
  char letter = 'a';
  int stop = 122;

  while(i<stop)
  {
    cout << letter << '\t' << ((int) letter) <<endl;
    i++;
    letter = 'a'+i;
  }

  return 0;
}
