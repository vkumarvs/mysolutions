#include <stdio.h>
#include <iostream>
#include <algorithm>

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

int InsertNode(LinkNode **head, int value)
{
    int length = 0;
    LinkNode *tmp = *head;
    if(!(*head))
    {
      *head = new LinkNode(value);
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


void PrintList(LinkNode *head)
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

/*
 Split the nodes of the given list into front and back halves,
     and return the two lists using the reference parameters.
     If the length is odd, the extra node should go in the front list.
     Uses the fast/slow pointer strategy.  
*/
void SplitList(LinkNode *head, LinkNode **a, LinkNode **b)
{
    if(!head)
      cout<<"head reference is null"<<endl;
    LinkNode *slowPtr = NULL, *fastPtr = NULL;
    slowPtr = head;
    fastPtr = head;
}

LinkNode * mergeList(LinkNode* a, LinkNode* b)
{
    printf("mergelist\n");
    LinkNode* head= new LinkNode(0);
    LinkNode *tail = head;
    while(true){
      if(a == NULL){
        printf("list a is complete adding rest of b\n");
        tail->nextNode = b;
        break;
      }
      if(b == NULL){
        printf("list b is complete adding rest of a\n");
        tail->nextNode = a;
        break;
      }
      if(a->GetValue() <= b->GetValue()){
        //keep appending smaller value into new head node
        tail->nextNode = a;
        //now move tail pointer to next
        tail = tail->nextNode;
        a = a->nextNode;
      }else{
        //keep appending smaller value into new head node
        tail->nextNode = b;
        //now move tail pointer to next
        tail = tail->nextNode;
        b = b->nextNode;
      }
    }
    return head->nextNode;
}

void mergeSort(LinkNode *list)
{
    LinkNode *a = NULL;
    LinkNode *b = NULL;

    if(!list || list->nextNode)
    {
      return ;
    }
    SplitList(list, &a, &b);
    mergeSort(a);
    mergeSort(b);
    list = mergeList(a,b);
}


int main()
{
    LinkNode *listA = NULL;
    LinkNode *listB = NULL;
    int listALength = 0;
    int listBLength = 0;
    //int arr1[] = {1,6,8,5,4};
    //int arr2[] = {8,4,12,18,14,6,1};
    int arr1[] = {1,4,5,6,8};
    int arr2[] = {1,4,6,8,12,14,18};
    for(int x = 0; x < (int)(sizeof(arr1)/4); x++)
    {
       listALength = InsertNode(&listA, arr1[x]);
    }

    for(int x = 0; x < (int)(sizeof(arr2)/4); x++)
    {
      listBLength = InsertNode(&listB, arr2[x]);
    }
    //PrintList(listA);   
    //cout<<"listA elements \t  " <<listALength <<"\t listB elements\t " <<listBLength  <<endl;
    //PrintList(listB);
 //   cout<<endl<<"merging"<<endl   ;
    //LinkNode *newList = merge(listA, listB, listALength, listBLength);
    LinkNode *newList = mergeList(listA,listB);
   // cout<<endl<<"After merging"<<endl   ;
    PrintList(newList);
    return 0;
}
