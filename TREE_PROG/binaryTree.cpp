#include "stdio.h"
#include "math.h"
#include <queue>
#include <stack>
#include <iostream>
using namespace std;

class BstTree
{
public:
    BstTree(int value)
    {
      m_leftChild = NULL;
        m_rightChild = NULL;
        m_parent = NULL;
      m_value = value;
    }
    ~BstTree();
    void SetValue(int value){ m_value = value;}
    int GetValue(){ return m_value;}
    BstTree *m_leftChild;
    BstTree *m_rightChild;
    BstTree *m_parent;
    int m_value;
};

int getHeight(BstTree *head)
{
    if(!head)
      return 0;
    int height1 = 0, height2 = 0;
    height1 = getHeight(head->m_leftChild);
    height2 = getHeight(head->m_rightChild);
    return (((height1 > height2)?height1:height2) + 1);
}
void traverseTillRootwithParentPtr(BstTree *node)
{
    if(!node)
    {
      printf("node pointer is invalid\n");
      return ;
    }
    BstTree *tmp = node;
    while(tmp!=NULL)
    {
      if(tmp->m_parent)
      {
        printf("child(%u) ---> parent(%u) \n", tmp->GetValue(), tmp->m_parent->GetValue());
      }
      else
      {
        printf("root value(%u) \n", tmp->GetValue());
      }
      tmp = tmp->m_parent;
    }
}

BstTree *searchTree(BstTree *head, int value)
{
    if(!head)
    {
      printf("head pointer is invalid\n");
      return NULL;
    }

    BstTree *tmp = head;
    while(tmp != NULL)
    {
      if(tmp->GetValue() == value)
      {
        printf("value is in the tree\n");
        return tmp;
      }
      if (tmp->GetValue() < value)
      {
        tmp = tmp->m_rightChild;
      }
      else if(tmp->GetValue() > value)
      {
        tmp = tmp->m_leftChild;
      }
    }

    if(!tmp)
    {
      printf("value does not exist\n");
    }
    return tmp;
}

bool createTree(BstTree *head, int value)
{
    if(!head)
    {
      printf("head pointer is invalid\n");
      return false;
    }

    BstTree *tmp = head;
    while(tmp != NULL)
    {
      if(value >= tmp->GetValue() )
      {
        if(tmp->m_rightChild == NULL)
        {
          tmp->m_rightChild = new BstTree(value);
          tmp->m_rightChild->m_parent = tmp;
          return true;
        }
        tmp = tmp->m_rightChild;
      }
      else
      {
        if(tmp->m_leftChild == NULL)
        {
          tmp->m_leftChild = new BstTree(value);
          tmp->m_leftChild->m_parent = tmp;
          return true;
        }
        tmp = tmp->m_leftChild;
      }
    }
    return false;
}

void printPreOrderTree(BstTree *node)
{
    if(!node)
    {
      return ;
    }
    printf("(%d) \t", node->GetValue());
    printPreOrderTree(node->m_leftChild);
    printPreOrderTree(node->m_rightChild);
}

void printBFSOrder(BstTree *node)
{
    BstTree *traverse = NULL;
    queue<BstTree *> myqueue;
    if(!node)
      cout <<"node element is null\n";
    cout <<"BFS order for tree is "<<endl;
    myqueue.push(node);
    while(!myqueue.empty())
    {
      traverse = myqueue.front();
      myqueue.pop();
      printf("(%d) \t", traverse->GetValue());
      if(traverse->m_leftChild != NULL)
      {
        myqueue.push(traverse->m_leftChild);
      }
      if (traverse->m_rightChild != NULL)
      {
        myqueue.push(traverse->m_rightChild);
      }
    }
      cout <<endl;
}

void printDFSOrder(BstTree *node)
{
    BstTree *traverse = NULL;
    stack<BstTree *> mystack;
    if(!node)
      cout <<"node element is null\n";
    cout <<"\nDFS order for tree is "<<endl;
    mystack.push(node);
    while(!mystack.empty())
    {
      traverse = mystack.top();
      mystack.pop();
      printf("(%d) \t", traverse->GetValue());
      if(traverse->m_leftChild != NULL)
      {
        mystack.push(traverse->m_leftChild);
      }
      if (traverse->m_rightChild != NULL)
      {
        mystack.push(traverse->m_rightChild);
      }
    }
      cout <<endl;
}

/*When we know the head pointer*/
void LeastCommonAncestor(BstTree *node, int value1 , int value2)
{
    if(!node)
    {
      printf("node is null:: no LCA found\n");
      return;
    }

    if((node->GetValue() < value1) && (node->GetValue() < value2))
    {
      LeastCommonAncestor(node->m_rightChild, value1, value2);
    }
    else if((node->GetValue() > value1) && (node->GetValue() > value2) )
    {
      LeastCommonAncestor(node->m_leftChild, value1, value2);
    }
    else
    {
      printf("Least common ancestor node val is %u \n" , node->GetValue());
      return;
    }
}
void deleteValue(int value)
{

}

void LeastCommonAncestorWithoutRoot(BstTree *node1, BstTree *node2)
{
    if(node1 == NULL)
    {
      printf("node1 pointers is  null:: no LCA found\n");
      return;
    }

    if(node2 == NULL)
    {
      printf("node2 pointers is  null:: no LCA found\n");
      return;
    }
    printf("i am inside LeastCommonAncestorWithoutRoot\n");

    if((node1->GetValue() == node2->GetValue()))
    {
      printf("Least common ancestor node val is %u \n" , node1->GetValue());
      return;
    }

    if(node1->m_parent && node2->m_parent)
    {
      if(node1->m_parent->GetValue() == node2->GetValue())
        LeastCommonAncestorWithoutRoot(node1->m_parent, node2);
      else if(node2->m_parent->GetValue() == node1->GetValue())
        LeastCommonAncestorWithoutRoot(node2->m_parent, node1);
      else
        LeastCommonAncestorWithoutRoot(node1->m_parent, node2->m_parent);
    }
    else if(!(node1->m_parent))
    {
      LeastCommonAncestorWithoutRoot(node1, node2->m_parent);
    }
    else if(!(node2->m_parent))
    {
      LeastCommonAncestorWithoutRoot(node1->m_parent, node2);
    }
}

#if 0
void SearchValueWthParentPointer(BstTree *node1, int value)
{
    if(!node)
    {
      printf("node pointer is null:: no LCA found\n");
      return;
    }

    if((node1->GetValue() < value1) && (node->GetValue() < value2))
    {
      LeastCommonAncestor(node->m_rightChild, value1, value2);
    }
    else if((node->GetValue() > value1) && (node->GetValue() > value2) )
    {
      LeastCommonAncestor(node->m_leftChild, value1, value2);
    }
    else
    {
      printf("Least common ancestor node val is %u \n" , node->GetValue());
      return;
    }
}
#endif

int main()
{
    char x;
    BstTree *head = NULL;
    printf("\nEnter options \n" "u - parent pointer traversal" "c - create node \n" "d - delete \n"
             "p-print\n" "s - search\n" "l - LCA\n" "a - create with array\n" "q - quit \n");
    while(1)
    {
      printf("\nEnter option value\n");
      scanf("%c", &x);
      if(x == 'q')
        break;
      switch(x)
      {
        case 'p':
        printf("tree structure is ---\n");
        printPreOrderTree(head);
        printf("\n");
        printf("tree height is --- %d\n", getHeight(head));
        cout <<endl;
        printBFSOrder(head);
        printDFSOrder(head);
        break;
        case 'l':
        {
          int value1 = 0, value2 = 0;
          printf("\nEnter two tree node values to find out common ancestor \n");
          scanf("%u %u", &value1, &value2);
          LeastCommonAncestor(head, value1 , value2);
        }
        break;
        case 's':
        {
          int value = 0;
          printf("Enter node value to be searched\n");
          scanf("%d", &value);
          BstTree *node1 = searchTree(head, value);
          if(node1)
          {
            printf("value(%u) is present in tree\n", node1->GetValue());
          }
          printf("Enter second value to be searched\n");
          scanf("%d", &value);
          BstTree *node2 = searchTree(head, value);
          if(node2)
          {
            printf("value(%u) is present in tree\n", node2->GetValue());
          }
          /*Find out LCA if we  know just node1 and node2 with help of parent 
           * pointer*/
          LeastCommonAncestorWithoutRoot(node1, node2);
          break;
        }
        case 'u':
        {
          int value = 0;
          printf("Enter node value to be searched\n");
          scanf("%d", &value);
          BstTree *node1 = searchTree(head, value);
          if(node1)
          {
            printf("value(%u) is present in tree\n\n", node1->GetValue());
          }
          traverseTillRootwithParentPtr(node1);
          break;
        }
        case 'd':
        {
          int value = 0;
          printf("Enter node value to delete\n");
          scanf("%d", &value);
          deleteValue(value);
          break;
        }
        case 'c':
        {
          int value = 0;
          printf("Enter node value\n");
          scanf("%d", &value);
          if(!head)
          {
            head = new BstTree(value);
          }
          else
          {
            createTree(head, value);
          }
        break;
        }
        case 'a':
        {
          //int array[] = {60,30,80,20,50,70,90,35,55,65,74,85,100,52,58,95};
          int array[] = {60,30,80,20,50,70,90,35,55,65};
          printf("size of array(%zu)\n", sizeof(array)/sizeof(int));
          head = new BstTree(array[0]);
          for(int x = 1; x < sizeof(array)/sizeof(int); x++)
          {
            createTree(head, array[x]);
          }
        break;
        }
        default:
        break;
      }
    }

    return 0;
}
