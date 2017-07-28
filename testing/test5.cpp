#include <iostream>

class foo
{
  public:
    foo (int a);
    ~foo();
    int my_int;
    int* myInt;
    void get_myInt(int* CopyMyInt);
};

foo::foo (int a)
{
  my_int = a;
  myInt = &my_int;
  int tmp = *myInt;
  std::cout << "Creating a foo object, myInt = " << tmp << std::endl;
}

foo::~foo()
{ 
  int tmp = *myInt;
  std::cout << "Destroying a foo object, myInt = " << tmp << std::endl;
}

void foo::get_myInt( int* CopyMyInt)
{
  *CopyMyInt = *myInt;
  return;
}

int main()
{

  int value = 0;
  int* copy = &value;
  std::cout << "*copy = " << *copy << std::endl;

  {
    foo bar( 1);
    bar.get_myInt( copy);
  }

  std::cout << "*copy = " << *copy << std::endl;

  
  return 0;
}
