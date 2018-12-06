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

void removeNthNodeFromLast(LinkNode *head, int n)
{
    printf("needs to remove n(%d)th node\n", n);
    if(!head)
      return ;

    LinkNode *currPtr = head, *prevPtr = head;

    while (n-- > 0)
    {
      if(currPtr->nextNode)
        currPtr = currPtr->nextNode;
     printf("value of currnode %d\n", currPtr->GetValue());   
    }

    while(currPtr->nextNode)
    {
      currPtr=currPtr->nextNode;
      prevPtr = prevPtr->nextNode;
    }

    printf("After -- value of prevnode (%d)\n", prevPtr->GetValue());   
    LinkNode *newNode = prevPtr->nextNode;
    prevPtr->nextNode = newNode->nextNode;
    delete newNode;
}

int main()
{
    LinkNode *listA = NULL;
    int listALength = 0;
    int arr1[] = {2,4,3,5,6,9,10};
    for(int x = 0; x < (int)(sizeof(arr1)/4); x++)
    {
       listALength = InsertNode(listA, arr1[x]);
    }

    PrintList(listA);   

    removeNthNodeFromLast(listA, 3);

    cout<<endl<<"After addition"<<endl   ;
    PrintList(listA);
    return 0;
}
