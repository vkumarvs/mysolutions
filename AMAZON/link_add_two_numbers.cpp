#include <stdio.h>
#include <iostream>
#include <algorithm>

using namespace std;


/*
You are given two linked lists representing two non-negative numbers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list. Input: (2 -> 4 -> 3) + (5 -> 6 -> 4) Output: 7 -> 0 -> 8 
*/
class LinkNode
{
public:
    LinkNode(int value)
    {
      m_value = value;
      nextNode = NULL;
    }
    ~LinkNode(){};
    int GetValue(){ return m_value;}
    LinkNode *nextNode;
private:
    int m_value;
};

int InsertNode(LinkNode *&head, int value)
{
    int length = 0;
    LinkNode *tmp = head;
    if(!(head))
    {
      head = new LinkNode(value);
      length++;
      return length;
    }

    length++;
    while(tmp->nextNode != NULL)
    {
      tmp = tmp->nextNode;
      length++;
    }
    tmp->nextNode = new LinkNode(value);
    return ++length;
}


void PrintList(LinkNode *&head)
{
    LinkNode *tmp = head;
    if(!tmp)
      cout<<"List does not have any element" <<endl;

    cout<<"printing list elements " <<endl;
    while(tmp!= NULL)
    {
      cout<<tmp->GetValue() <<"\t"; 
      tmp = tmp->nextNode;
    }
    cout<<endl;
}

LinkNode *addNodes(LinkNode *l1, LinkNode *l2, int carry)
{
    if(!l1 && !l2 && carry==0) return NULL;
    int a=0, b=0;
    if(l1){ a=l1->GetValue(); }
    if(l2){ b=l2->GetValue(); }
    int value=a+b+carry;
    LinkNode *head=new LinkNode(value%10);
    head->nextNode=addNodes(l1? l1->nextNode: NULL , l2?l2->nextNode:NULL, value/10);
    return head;
}

LinkNode *addTwoNumbers(LinkNode *l1, LinkNode *l2) 
{
  // Start typing your C/C++ solution below
  // DO NOT write int main() function
  return addNodes(l1, l2, 0);
}

int main()
{
    LinkNode *listA = NULL;
    LinkNode *listB = NULL;
    int listALength = 0;
    int listBLength = 0;
    int arr1[] = {2,4,3};
    int arr2[] = {5,6,4,7};
    for(int x = 0; x < (int)(sizeof(arr1)/sizeof(int)); x++)
    {
       listALength = InsertNode(listA, arr1[x]);
    }

    for(int x = 0; x < (int)(sizeof(arr2)/sizeof(int)); x++)
    {
      listBLength = InsertNode(listB, arr2[x]);
    }
    PrintList(listA);   
    PrintList(listB);

    LinkNode *newList = addTwoNumbers(listA,listB);

    cout<<endl<<"After addition"<<endl   ;
    PrintList(newList);
    return 0;
}
