#include<stdio.h>
#include<string.h>
#include <string>
using namespace std;

void revrseRecur(char str[],int start, int end)
{
    //printf("%s start(%d) end(%d)\n",str,start,end); 
    if(start >= end)
    {
      return; 
    }
    char temp;
    temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
    revrseRecur(str,start,end); 
}

void revrse(char str[],int start, int end)
{
    char temp;
    //printf("%s start(%d) end(%d)\n",str,start,end); 
    if(start >= end)
    {
      return; 
    }
    while(end > start)
    {
      temp = str[start];
 //     printf("start(%d) end(%d) temp value is %c --- %c\n", start, end , str[start], str[end]);
      str[start] = str[end];
      str[end] = temp;
      start++;
      end--;
    }
}

void revrseWord(char str[],int start, int end)
{
    char temp;
    if(start >= end)
    {
      return; 
    }

    revrse(str,start,end);
    int tmp = start;
    int startWordIndex = start;
    int endWordIndex = start;
    while(tmp!=end)
    {
      if(str[tmp] == ' ')
      {
        revrse(str, startWordIndex, (endWordIndex - 1));
        startWordIndex = endWordIndex+1;
      }
      else if(endWordIndex == end)
      {
        revrse(str, startWordIndex, end);
      }
      endWordIndex++;
      tmp++;
    }
}

int main()
{
    char str[] = "hi i am vipin dahiya";
    printf("string length is %Zu\n", strlen(str));
    printf("before--%s \n",str); 
    revrse(str,0,strlen(str) - 1);
    //revrseRecur(str,0,strlen(str) - 1);
    //printf("before--%s \n",str); 
    //revrseWord(str,0,strlen(str) - 1);
    printf("After --%s \n",str); 

    return 0;

}
