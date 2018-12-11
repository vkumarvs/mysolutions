#include <stdio.h>
#include<iostream>
#include <stack>
#include <string>
using namespace std;
#if 0
struct LinkNode
{
 LinkNode *next;
 int value;
};


LinkNode * AddTwoLinkList(LinkNode *l1, LinkNode *l2, int carry)
{
    LinkNode *node = new LinkNode();
    if(!l1 && !l2 && carry==0)
      return NULL;
    int a=0,b=0;
    if(!l1) a= l1->value;
    if(!l2) b= l2->value;
    resut = a+b+carry;
    node->value = result%10;
    node->next = AddTwoLinkList(l1?l1->next:NULL,l2?l2->next:NULL,result/10);
    return node;
}


bool isPalindrome(int num)
{
    int reverse = 0;
    while(num !=0)
    {
      reverse = reverse*10+num%10;
      num = num/10;
    }
    if(reverse > INT_MAX || reverse < INT_MIN)
    {
      return false;
    }

    if(reverse == num)
      return true;
    else
      return false;
}

LinkNode *node removeNthNode(LinkNode *head, int n)
{
   LinkNode *fast=head, *slow = head;
   while(n-- > 0)
   {
     if(fast->next)
     fast = fast->next;
   }
   
   while(fast->next)
   {
     slow = slow->next;
     fast = fast->next;
   }

   //check if number n is not beyond the linklist limit
   slow->next = slow->next->next;
    return head;
}
#endif

void removeDuplicate()
{

}

int main()
{
   string test = "()[]{}";  
   bool val = validatestring(test);
   printf("val %s\n", val?"TRUE":"FALSE");
}













