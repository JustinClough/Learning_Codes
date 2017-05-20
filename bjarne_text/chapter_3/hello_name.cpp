#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::string;

int main()
{

  cout << "Please enter your first name (followed by 'return'): \n" ;
  string first_name;
  cin >> first_name;
  cout << "Hello, " << first_name << "!\n";

}
