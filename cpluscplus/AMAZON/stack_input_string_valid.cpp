#include <stdio.h>
#include<iostream>
#include <stack>
#include <string>

using namespace std;
bool validatestring(string &test)
{
    if(test.length() == 0 || test.length() ==1)
    {
      return false;
    }
    char c, next ;
    stack<char> mystack;
    for(int x = 0; x < test.length(); x++)
    {
      c = test.at(x);
      if(!mystack.empty())
      {
        next = mystack.top();
        printf("at the top %c \n", next);
      }
      switch(c)
      {
        case '{':
        case '(':
        case '[':
        mystack.push(c);
        break;
        case '}':
        if(next != '{')
          return false;
        else
          mystack.pop();
        break;
        case ')':
        if(next != '(')
          return false;
        else
          mystack.pop();
        break;
        case ']':
        if(next != '[')
          return false;
        else
          mystack.pop();
        break;
        default:
        printf("done\n");
        return false;
      }
    }
    return true;
}

int main()
{
   string test = "()[]{}";  
   bool val = validatestring(test);
   printf("val %s\n", val?"TRUE":"FALSE");
}

