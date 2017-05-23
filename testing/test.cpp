#include <iostream>
#include <string>


using std::cout;
using std::endl;
using std::string;

// Base Class Definition
class Book
{
  public:
    string title;
    int pages;
    Book(string t, int n);
};

Book::Book(string t, int n)
{
  title = t;
  pages = n;
}

class Text_book : public Book
{
  public:
    string subject;
    Text_book( string s, string t, int n);
};

Text_book::Text_book( string s, string t, int n) : Book ( t, n)
{
  subject = s;
}

// Class Definition
class Square
{
  private:
    int length;
  public:
    Square();
    Square(int);
    int height;
    int area ()
    {
      return length*length;
    }
    void set_h()
    {
      height = length;
    }
};

// Default Square constructor
Square::Square ()
{
  length = 10;
  set_h();
}

// Square constructor with user input
Square::Square( int a)
{
  length = a;
  set_h();
}


int main( int argc, char** argv ) 
{
  // Declare an integer b and define value as 5
  int b = 5;

  // Declare an integer pointer a, define it to point to the address of b
  int* a = &b;

  // Iterate from 0 to the value pointed to by a
  for(int i=0; i<*a; i++)
  {
    std::cout << "i = " << i << std::endl;
  }

  Square sq_def;
  cout << "Default Area = " << sq_def.area() << ". With length =" << sq_def.height << endl;

  Square sq1 (5);
  cout << "Declared Area = " << sq1.area() << ". With length =" << sq1.height << endl;

  Text_book book1 ( "subject", "title" , 100);
  
  cout << "Book 1: Subject=" << book1.subject << " Title=" << book1.title
      << " Pages=" << book1.pages << endl;

  return 0;
}
