#include <iostream>

class c_one_t
{
  public:
    c_one_t (int b);
    int a;
    void print()
    { std::cout << a << std::endl;}
};

class c_two_t
{
  public:
    c_two_t (int d);
    int c;
    void print()
    { std::cout << c << std::endl;}
    c_one_t* three;
};

c_one_t::c_one_t( int b)
{
  a = b;
}

c_two_t::c_two_t( int d)
{
  c = d;
  c_one_t four ( d); 
  three = &four;
}

int main()
{
  c_one_t one (10);
  one.print();
  
  c_two_t two (20);
  two.print();

  two.three->print();


  return 0;
}
