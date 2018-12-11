#include<stdio.h>
#include<string.h>
void revrse(char str[],int start, int end)
{
   char *temp;
   printf("%s start(%d) end(%d)\n",str,start,end); 
   if(start >= end)
      {
        return; 
      }
   *temp = *(str+start);
   *(str+start) = *(str+end);
   *(str+end) = *temp;
   start++;
   end--;
   revrse(str,start,end); 
}
int main()
{
  char str[] = "hi i am vipin dahiya ";
   revrse(str,0,strlen(str)-1);
   printf("%s \n",str); 
   
  return 0;

}
