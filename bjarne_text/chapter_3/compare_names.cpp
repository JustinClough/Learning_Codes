#include "bjarne.hpp"

int main()
{
  cout << "Please enter two names\n:";
  string first;
  string second;
  cin >> first >> second;
  if( first == second)
  {
    cout << " That's the same name twice!\n";
  }
  if ( first < second)
  {
    cout << first << " is alphabetically before " << second << endl;
  }
  if (first > second)
  {
    cout << first << " is alphabetically after " << second << endl;
  }
  return 0;

}
