#include <iostream>
//reverse singly link list
using namespace std;

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

void reverseLinkList(LinkNode *&head)
{
  if(!head || !(head->nextNode)) 
   return;
  LinkNode *cur = head, *prev = NULL, *next=NULL;
  while(cur != NULL)
  {
    next = cur->nextNode;
    cur->nextNode = prev;
    prev = cur;
    cur = next;
  }
  head = prev;
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

    reverseLinkList(listA);

    cout<<endl<<"After reversal"<<endl   ;
    PrintList(listA);
    return 0;
}


