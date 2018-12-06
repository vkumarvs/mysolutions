#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool isMatch(string &pattern, string &sample)
{
    if(pattern.size() == 0)
    {
      return false;
    }
    if(sample.size() == 0)
    {
      return false;
    }

    vector<char> pat(pattern.begin(), pattern.end());
    vector<char> sam (sample.begin(), sample.end());
    int pStartPos = 0, sStartPos = 0;
    char matchChar ;
    for(int i = 0; i < sample.size(); i++)
    {
      if(((pat[pStartPos] == '*')) && (pat.size() == (pStartPos + 1)))
      {
        return true;
      }
      else
      {
        matchChar = pat[(pStartPos + 1)];
      }
     
      if((pat[pStartPso] == '*'))
      {

      }
      else
      {
      }

    }

    return true;
}


int main()
{
    //string pattern("abc*def*h");
    string pattern("*");
    //string pattern("**");
    string sample1("*");
    string sample2("accsyzedefhhhhhhp");
    cout<<"sample 1 -----"<<(isMatch(pattern, sample1)?"match":"not a match");
    cout<<endl;
    cout<<"sample 2 -----"<<(isMatch(pattern, sample2)?"match":"not a match");
    cout<<endl;
    return true;
}
