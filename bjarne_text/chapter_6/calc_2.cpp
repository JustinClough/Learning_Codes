#include <iostream>
#include <string>
#include <cstdlib>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::abort;

void print_error(string message)
{
  cout << message << endl;
  abort();
  return;
}

int main()
{
  cout << "Please enter expression (+, -, *, and / only): ";
  cout << "Add an x to end of expression to execute:" << endl;
  int lval;
  int rval;
  char op;

  cin >> lval;
  if(!cin)
  {
    print_error("No first operand");
  }

  for (op; cin>>op;)
  {
    switch(op)
    {
      case '+':
        lval+=rval;
        break;
      case '-':
        lval-=rval;
        break;
      case '*':
        lval*=rval;
        break;
      case '/':
        lval/=rval;
        break;
      default:
        cout << "Result: " << lval << endl;
        return 0;
    }
  }
  print_error("Bad Expression.");
}
