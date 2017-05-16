#include <iostream>

class square{
  private:
    int length;
  public:
    square();
    square(int);
    int height;
    int area (void)
    {
      return length*length;
    }
};

square::square ()
  {
  length = 10;
  }

square::square( int a)
  {
    length = a;
  }

int main( int argc, char** argv ) {

  using std::cout;
  using std::endl;

  int b = 5;
  int* a = &b;

  for(int i=0; i<*a; i++)
  {
    std::cout << "i = " << i << std::endl;
  }

  square sq_def;

  cout << "Default Area = " << sq_def.area() << endl;

  square sq1 (5);

  cout << "Declared Area = " << sq1.area() << endl;

  return 0;
}
