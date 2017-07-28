#include <iostream>

class obj
{
  public:
    obj ( int value);
    ~obj();
    void print_int();
    void change_int( int new_int);
    int myInt;
    int* pInt;
};

obj::obj( int value)
{
  myInt = value;
  pInt = &myInt;
}

obj::~obj()
{
  std::cout << "Destroying obj, myInt = " << myInt << std::endl;
}

void obj::print_int()
{
  std::cout << "myInt is " << myInt << std::endl;
  return;
}

void obj::change_int( int new_int)
{
  myInt = new_int;
  return;
}

int main()
{

  obj* original = new obj ( 5);
  original->print_int();

  obj copy (*original);
  copy.print_int();
  copy.change_int( 2);
  copy.print_int();

  delete original;

  return 0;
}
