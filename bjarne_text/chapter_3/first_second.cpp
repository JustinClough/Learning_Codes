#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::cin;
using std::endl;

int main()
{
  cout << "Please enter your first and second names\n";
  string first;
  string second;
  cin >> first >> second;
  string name = first + ' ' + second;
  cout << "Hello, " << name << endl; 

  return 0;
}
