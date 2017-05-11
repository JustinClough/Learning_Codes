//written for: Practice project to include multiple source files and header files. 
//written by: Justin Clough
//Written on: 10/10/2016

#include <iostream> //Include iostream library
#include "algebra.h" //Include algebra library 

// declare variables and initial values
int a = 1;
int b = 1;

//begin main function
int main () 
{

	// Ask user for input 
  std:: cout << "Input an integer:" << std::endl;
  std:: cin >> a;
  std:: cout << "Input another, non-zero, integer:" << std::endl;
  std:: cin >> b; 

  
  // Crunch numbers using functions defined in 'algebra' header
  int Sum =  Addition(a, b);
	int Product = Multiply(a, b);
	int Differ = Subtraction(a, b);
	double Dividen = Division(a, b);

	// Report results to user
	std:: cout << "The sum is:" << Sum << std:: endl;
	std:: cout << "The product is: " << Product << std:: endl;
	std:: cout << "The difference is: " << Differ << std:: endl;
	std:: cout << "The dividen is: " << Dividen << std::endl;

	//End main funciton
	return 0;
}




