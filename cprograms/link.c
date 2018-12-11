#include<stdio.h>
#include <stdlib.h>

typedef struct _link
{
  int data;
  struct _link *next;

}Link;

void addNode(Link **head,int x);
void traverse(Link *head);

int main()
{ 
  int x,y;
  Link *head = NULL ;
  printf("what operation yo need ::\n 1: addNode\n 2: traverse   \n");
  scanf("%d",&x);

  switch(x)
  {
   case 1:
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
   case 2:
      traverse(head);
     break;
   default :
    printf("Default reached \n");
 }
  return 0;
}
void addNode(Link **head,int x)
{
    if(!(*head))
      {
        (*head)= malloc(sizeof(Link));
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
        (temp)->next= malloc(sizeof(Link));
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
