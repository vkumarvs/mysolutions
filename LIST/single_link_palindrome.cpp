#include<stdio.h>
#include <stdlib.h>
#include <stack>
using namespace std;
typedef struct _link
{
  int data;
  struct _link *next;
  struct _link *random;

}Link;

void addNode(Link **head,int x);
void traverse(Link *head);
bool isPalindrome(Link *head);
void reverseList(Link *head);
void randomPointerCopy(Link *head);

int main()
{ 
  int x,y;
  char in;
  Link *head = NULL ;
  printf("operation ::\n a: create list from arr \n c: addNode\n p: traverse \n r: reverse \n x: copy list with random pointer");

  while(1)
  {
    printf("Enter operation option\n");
    scanf("%c",&in);
    switch(in)
    {
      case 'c':
      {
        int i,l;
        printf("Enter how many element to enter  \n");
        scanf("%d",&y);
        for(i=0;i<y;i++)
        {
          printf("Enter element   \n");
          scanf("%d",&l);
          addNode(&head,l);
        }
      }
      //break;
      case 'p':
      traverse(head);
      break;

      case 'r':
      reverseList(head);
      break;

      case 'x':
      randomPointerCopy(head);
      break;

      case 'a':
      {
        printf("enter input abt odd and even number element\n");
        int element = 0;
        int size = 0;
        scanf("%d", &element);
        int arr1[] = {1,2,3,3,2,1};
        int arr2[] = {1,2,3,4,3,2,1};
        if(element%2 == 0)
        {
          size = sizeof(arr1)/sizeof(int);
          for(int x = 0; x < size; x++)
          {
            addNode(&head, arr1[x]);
          }
        }
        else
        {
          size = sizeof(arr2)/sizeof(int);
          for(int x = 0; x < size; x++)
          {
            addNode(&head, arr2[x]);
          }
        }
        break;
      }
      case 'd':
      {
        printf("isPalindrome %s\n", isPalindrome(head)?"true":"false");
        break;
      }
      case 'm':
      {
        //printf("middle element %d\n", middleElement(head));
        break;
      }
      case 'q':
      {
        exit(0);
      }
      default :
      printf("Default reached \n");
    }
  }
  return 0;
}
void addNode(Link **head,int x)
{
    if(!(*head))
      {
        (*head)= (Link *)malloc(sizeof(Link));
        (*head)->data = x;
        (*head)->next = NULL;
      }
    else
 {
    Link *temp;
    temp = *head;
    while(temp->next != NULL)
     {
          temp  = temp->next;
     } 
        (temp)->next= (Link *)malloc(sizeof(Link));
        (temp)->next->data = x;
        (temp)->next->next = NULL;

  } 
  return;
}
void traverse(Link *head)
{
  if(!head)
    {
      return;
    }
   else
 {
    while(head != NULL)
     {   
         printf("%d-------->",head->data);
          head = head->next;
     } 
  }
         printf("\n");
return; 
   
}

int middleElement(Link *head)
{
}

/*Be careful for first 1, 2, 3 and 4 element in list
 * check all boundary conditions*/
bool isPalindrome(Link *head)
{
    if(!head)
    {
      return false;
    }

    stack<int> mystack;
    Link *fastPtr = head;
    Link *slowPtr = head;
    Link *tmp = fastPtr;
    while(tmp)
    {
      /*push data to stack*/
      mystack.push(slowPtr->data) ;
      slowPtr = slowPtr->next;
      if(!slowPtr)
      {
        break;
      }
      else
      {
        tmp = fastPtr->next->next;
        fastPtr = fastPtr->next->next;
      }

      /*If there are just two elements*/
      if(fastPtr == NULL)
      {
        printf("top data %d\n", mystack.top());
        if(mystack.top() != slowPtr->data)
        {
          return false;
        }
        else
        {
          return true;
        }
      }

      if(fastPtr->next == NULL) //odd element case
      {
        /*push data to stack*/
        mystack.push(slowPtr->data) ;

        //pop slow pointer element and start comparing next elements
        slowPtr = slowPtr->next;
        /*it represet if there are just three element in list*/
        if(slowPtr != fastPtr)
        {
          mystack.pop();
        }
        while(slowPtr)
        {
          if(mystack.top() != slowPtr->data)
          {
            return false;
          }
          else
          {
            if(mystack.size() > 0)
            {
              mystack.pop();
            }
            slowPtr = slowPtr->next;
          }
        }
        return true;
      }
      else if(fastPtr->next->next == NULL) //even element case
      {
        /*push data to stack*/
        mystack.push(slowPtr->data) ;
        slowPtr = slowPtr->next;
        while(slowPtr)
        {
          if(mystack.top() != slowPtr->data)
          {
            return false;
          }
          else
          {
            mystack.pop();
            slowPtr = slowPtr->next;
          }
        }
        return true;
      }
    }
    return true;
}

//http://tech-queries.blogspot.in/2011/04/copy-linked-list-with-next-and-random.html#
Link* randomPointerCopy(Link* root)  
{  
    Link *res;  

    Link* cur = root;  
    Link *next, *tmp;  

    //Create the copy of every node in the Link and insert   
    //it in original Link between current and next node.   
    while(cur)  
    {  
      tmp = new(Link);  
      tmp->val = cur->val;  
      tmp->random = NULL;  
      tmp->next = cur->next;  
      next = cur->next;  
      cur->next = tmp;  
      cur = next;          
    }  

    //save result pointer      
    res = root->next;  

    //Copy the arbitrary link for result  
    cur = root;      
    while(cur)  
    {  
      cur->next->random = cur->random->next;  
      cur = cur->next->next;  //move 2 nodes at a time  
    }  

    //restore the original and copy linked lists  
    cur = root;      
    tmp = root->next;  
    while(cur && tmp)  
    {  
      cur->next = cur->next->next;  
      cur = cur->next;  
      if (tmp->next){  
        tmp->next = tmp->next->next;  
        tmp = tmp->next;  
      }          
    }  

    return res;  
}  

void reverseList(Link *head)
{

}
