#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

int main()
{
  vector<string> bad_words;
  bad_words.push_back("bad");
  bad_words.push_back("words");

  vector<string> sentence;
  string t;
  while(cin >> t)
  {
    sentence.push_back(t);
  }
  
  vector<string> censored;
  for(int j=0; j<sentence.size(); j++)
  {
    bool is_bad = false;
    for(int i=0; i<bad_words.size();i++)
    {
      if(bad_words[i] == sentence[j])
      {
        is_bad = true;
      }
    }
    if(is_bad)
    {
      censored.push_back( "BLEEP");
    }
    else
    {
      censored.push_back(sentence[j]);
    }
  }

  for(int i=0; i<censored.size(); i++)
  {
    cout << censored[i] ;
    cout << " " ;
  }

  cout << endl;

  return 0;
}
