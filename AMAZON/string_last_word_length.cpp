#include <stdio.h>
#include <string>
using namespace std;


int last_word_length(const char* str, int length)
{
    printf("string length is %d\n", length);
    if(length == 0)
      return 0;

    int count = 0;
    bool flag=false;
    for(int i = (length-1) ; i >=0; i--)
    {
      char c = str[i];
      printf("char is %c\n", c);
      if((c >= 'a' && c <= 'z') || (c >='A' && c <= 'Z'))
      {
        flag = true;
        count++;
      }
      else
      {
  printf("length %d\n", count);
      if(flag)
       return count;
      }
    }
  return count;
}

int main()
{
   //string str = "hello my name ";
   string str = "hello my name ";
   int length = str.length();
   last_word_length(str.c_str(), length);
    return 0;
}
