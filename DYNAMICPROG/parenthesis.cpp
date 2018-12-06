#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> generateParenthesis(int count)
{
    vector<string> set;
    vector<string>::iterator x;
    if(count == 1)
    {
      set.push_back("()");
      return set;
    }
    else
    {
      if(count-1 != 0)
        set = generateParenthesis((count - 1));

      vector<string> newSet;

      for(x = set.begin(); x != set.end(); x++)
      {
        string tmp ;
        for(int y = 0; y < ((*x)).length(); y++)
        {
          if(((*x))[y] == '(')
          {
            tmp = ((*x)).substr(0, y+1) + "()" + ((*x)).substr(y+1 , ((*x)).length());
             //TODO::one correction push only if it is not duplicate
             /*Also this data structure is not suitable :: use SET*/
            newSet.push_back(tmp);
          }
        }
        newSet.push_back("()" + tmp);
      }
    return newSet;
    }
}

int main()
{
    vector<string> parenth = generateParenthesis(2);
    for(auto x = parenth.begin(); x != parenth.end(); x++)
    {
      cout<<"\""<<(*x)<<"\""<<endl;
    }
    return 0;
}
