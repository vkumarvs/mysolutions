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

bool detectCycle(LinkNode *head)
{
  if(!head || !(head->nextNode)) 
   return false;
  LinkNode *slow = head, *fast = head;
  while(fast!= NULL && fast->nextNode!=NULL)
  {
    slow=slow->nextNode;
    fast = fast->nextNode->nextNode;
    if(slow==fast)
      return true;
  }
  return false;
}

int main()
{
    LinkNode *listA = NULL;
    int listALength = 0;
    int arr1[] = {1,2,3,4,5};
    for(int x = 0; x < (int)(sizeof(arr1)/4); x++)
    {
      listALength = InsertNode(listA, arr1[x]);
    }

     /* Create a loop for testing */
        listA->nextNode->nextNode->nextNode->nextNode->nextNode = listA->nextNode->nextNode;

   // PrintList(listA);   
   
    if(detectCycle(listA))
    {
      cout<<endl<<"List Has Loop"<<endl   ;
    }
    else
    {
      cout<<endl<<"List doesn't has loop"<<endl   ;
    }
    return 0;
}


