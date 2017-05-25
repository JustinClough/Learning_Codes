#include <iostream>

using std::cout;
using std::cin;
using std::endl;

class Token
{
  public:
    char kind;
    double value;
};

int main()
{
  cout << "Please enter expression (+ and - only): ";
  int lval = 0;
  int rval = 0;
  char op = '+';

  cin >> lval >> op >> rval;

  int res;
  if(op=='+')
  {
    res = lval+rval;
  }
  else if (op=='-')
  {
    res = lval-rval;
  }

  cout << "Result: " << res << endl;

  return 0;
}
