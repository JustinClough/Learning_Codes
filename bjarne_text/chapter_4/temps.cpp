#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

int main()
{
  vector<double> temps;
  for(double temp; cin>>temp;)
  {
    temps.push_back(temp);
  }

  double sum = 0.0;
  for(double x:temps)
  {
    sum+=x;
  }
  cout << "Average Temp==" << sum/temps.size() << endl;

  sort(temps.begin(), temps.end());
  cout << "Median Temp==" << temps[temps.size()/2] << endl;

  return 0;
}
