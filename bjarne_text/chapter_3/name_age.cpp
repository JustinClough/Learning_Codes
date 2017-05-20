#include <iostream>
#include <string>

using std::cout; 
using std::cin;
using std::string;


int main()
{

  cout << "Please Enter your First Name and age (followed by return):\n" ;
  string first_name;
  double age;
  cin >> first_name >> age;
  
  cout << "Hello, " << first_name << " (age " << age << ")!\n";
  cout << "(That's " << age << " years or " << age*12 << " months.)\n";
  

  return 0;

}
