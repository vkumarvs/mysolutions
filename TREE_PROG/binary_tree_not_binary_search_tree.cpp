#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <queue>

#include <limits.h>
#include "PrettyBinaryPrint.hpp"

using namespace std;


/*
What is the basics algorithm for testing if a tree is symmetrical. Because it is a binary tree, I would assume that it would be a recursive definition of sorts
The formal question is below:
A binary tree is a mirror image of itself if its left and right subtrees are identical mirror images i.e., the binary tree is symmetrical. This is best explained with a few examples.

  1
 / \
2   2

TRUE 

   1
  / \
 2   2
  \
   3

FALSE

     1
   /   \
  2     2
 / \   / \
4   3 3   4

TRUE

       1
     /   \
    2     2 
   / \   / \
  3   4 3   4

FALSE

       1
     /   \
    2     2
   /       \
  3         3

TRUE

*/

#if 0
void myparent(Btree *tree, int myid, Btree **parent){
  if(tree->id==(myid/2))
    *parent = tree;
  if(tree->left)
    myparent(tree->left, myid, parent);
  if(tree->right)
    myparent(tree->right, myid, parent);
}
void insert(Btree **tree, Btree *item){
  Btree *parent;
  if(item->id==1)
    *tree=item;
  else{
    myparent(*tree, item->id,&parent);
    if((item->id)%2)
      parent->right=item;
    else
      parent->left=item;
  }
}
#endif
void myparent(Btree *node, int myId, Btree **parent)
{
    if(node->id==(myId/2))
    {
      *parent = node;
      // printf("myID is %u  -- node id is %u\n",  myId, node->id);
    }
    if(node->left)
      myparent(node->left, myId, parent);
    if(node->right)
      myparent(node->right, myId, parent);
}

void maxHeapifyUp (Btree *node)
{
    while(node)
    {
      if(node->parent->val <= node->val)
      {
        int tmpVal = node->val;
        node->val = node->parent->val;
        node->parent->val = tmpVal;
        node = node->parent;
        if(node->parent == NULL)
        {
          break;
        }
      }
      else
      {
        break;
      }
    }
}

void maxHeapifyDown(Btree *node)
{
    while(node)
    {
      if(node->left && node->right)
      {
        if((node->left->val >= node->right->val) &&
           (node->left->val > node->val))
        {
          int tmpVal = node->val;
          node->val = node->left->val;
          node->left->val = tmpVal;
          node = node->left;

        }
        else if((node->right->val >= node->left->val) &&
                (node->right->val > node->val))
        {
          int tmpVal = node->val;
          node->val = node->right->val;
          node->right->val = tmpVal;
          node = node->right;
        }
      }
      else if(node->left == NULL && node->right !=NULL && (node->right->val >
                                                           node->val))
      {
        int tmpVal = node->val;
        node->val = node->right->val;
        node->right->val = tmpVal;
        node = node->right;
      }
      else if(node->left != NULL && node->right ==NULL && (node->left->val >
                                                           node->val))
      {
        int tmpVal = node->val;
        node->val = node->left->val;
        node->left->val = tmpVal;
        node = node->left;
      }
      else
      {
        break;
      }
    }
}

void fix_node_position_minHeap (Btree *node)
{
    while(node)
    {
      if(node->parent->val >= node->val)
      {
        int tmpVal = node->val;
        node->val = node->parent->val;
        node->parent->val = tmpVal;
        node = node->parent;
        if(node->parent == NULL)
        {
          break;
        }
      }
      else
      {
        break;
      }
    }
}

void insert(Btree *head, int value, int myId)
{
    if(!head)
    {
      printf("head pointer is invalid\n");
      return ;
    }

    if(myId == 1) //id 1 is for root node
    {
      printf("invalid id for node\n");
      return ;
    }
    Btree *parent;
    myparent(head, myId, &parent);
    if((myId)%2)
      parent->right = new Btree(value, myId);
    else
      parent->left= new Btree(value, myId);;
}

void heapPush(Btree *head, Btree *last, int value)
{
    /*Add the element at bottom and then heapify it up*/
    if(!head)
    {
      printf("head pointer is invalid\n");
      return ;
    }

}
Btree * heapPop(Btree *head, Btree *last, int &popedVal)
{
    if(!head || !last)
    {
      printf("head pointer is invalid\n");
      return 0;
      //return NULL;
    }
    Btree *parent;
    int lastElemId = (last->id - 1);
    myparent(head, lastElemId, &parent);
    //printf("heappop parent id is %u value is %u\n", parent->id, parent->val);

    popedVal = head->val;
    head->val = last->val;
    if(last->parent->left->val == last->val)
      last->parent->left = NULL;
    else
      last->parent->right = NULL;

    //delete (*last1);

    /*Update last element pointer*/
    if((parent->left != NULL) && (parent->left->id == lastElemId))
    {
      last = parent->left;
    }
    else if((parent->right != NULL) && (parent->right->id == lastElemId))
    {
      last = parent->right;
    }

    printf("1:last item value is %u id is %u\n", last->val, last->id);
    /*For deletion we heapify in down direction*/
    maxHeapifyDown(head);

    return last;
}

Btree *insertMinHeapElem(Btree *head, int value, int myId)
{
    if(!head)
    {
      printf("head pointer is invalid\n");
      return NULL;
    }

    if(myId == 1) //id 1 is for root node
    {
      printf("invalid id for node\n");
      return head;
    }

    /*Myparent function recursively find out right parent node*/
    Btree *parent;
    myparent(head, myId, &parent);
    Btree *curr = new Btree(value, myId);

    if((myId)%2)
    {
      parent->right = curr;
      curr->parent = parent;
      //printf("right: curr %u ------> parent %u \n", curr->val, curr->parent->val);
      fix_node_position_minHeap(curr);
      //printf("after fix right: curr %u ------> parent %u \n", curr->val, curr->parent->val);
    }
    else
    {
      parent->left= curr ;
      curr->parent = parent;
      //printf("left : curr %u ------> parent %u \n", curr->val, curr->parent->val);
      fix_node_position_minHeap(curr);
     // printf("after fix left : curr %u ------> parent %u \n", curr->val, curr->parent->val);
    }
    return curr;
}

Btree *insertMaxHeapElem(Btree *head, int value, int myId)
{
    if(!head)
    {
      printf("head pointer is invalid\n");
      return NULL;
    }

    if(myId == 1) //id 1 is for root node
    {
      printf("invalid id for node\n");
      return head;
    }

    /*Myparent function recursively find out right parent node*/
    Btree *parent;
    myparent(head, myId, &parent);
    Btree *curr = new Btree(value, myId);

    if((myId)%2)
    {
      parent->right = curr;
      curr->parent = parent;
    }
    else
    {
      parent->left= curr ;
      curr->parent = parent;
    }

    /*heapify up*/
    maxHeapifyUp(curr);
    return curr;
}

bool isMirrorImage(Btree *leftTree, Btree *rightTree)
{
    printf("isMirrorImage:;left val %u ---- right val %u\n", leftTree->val, rightTree->val);
    if(leftTree->val != rightTree->val)
    {
      printf("return:left val %u ---- right val %u\n", leftTree->val, rightTree->val);
      return false;
    }
    else
    {
      printf("left val %u ---- right val %u\n", leftTree->val, rightTree->val);
    }
    if((leftTree->left != NULL) && (rightTree->right != NULL))
    {
      if(!(isMirrorImage(leftTree->left, rightTree->right)) ||
         !(isMirrorImage(leftTree->right, rightTree->left)))
      {
        return false;
      }
    }
    else if((leftTree->left != NULL) && (rightTree->right == NULL))
    {
      return false;
    }
    else if((leftTree->left == NULL) && (rightTree->right != NULL))
    {
      return false;
    }
    return true;
}

void printPreOrderTree(Btree *node){
  if(!node)
  {
    return ;
  }
  printf("%d\t", node->val);
  printPreOrderTree(node->left);
  printPreOrderTree(node->right);
}

void printInOrderTree(Btree *node){
  if(!node)
  {
    return ;
  }
  printInOrderTree(node->left);
  printf("%d\t", node->val);
  printInOrderTree(node->right);
}

void postOrderTree(Btree *node){
  if(!node)
  {
    return ;
  }
  postOrderTree(node->left);
  postOrderTree(node->right);
  printf("%d\t", node->val);
}

void printBFSOrder(Btree *treeNode)
{
    Btree *traverse = NULL;
    queue<Btree *> myqueue;
    if(!treeNode)
      cout <<"treeNode element is null\n";
    cout <<"BFS order for tree is "<<endl;
    myqueue.push(treeNode);
    while(!myqueue.empty())
    {
      traverse = myqueue.front();
      myqueue.pop();
      printf("(%d) \t", traverse->val);
      if (traverse->right != NULL)
      {
        myqueue.push(traverse->right);
      }
      if(traverse->left != NULL)
      {
        myqueue.push(traverse->left);
      }
    }
    cout <<endl;
}


bool isBtreeBst(Btree *node, int imin, int imax)
{
    if(!node)
    {
      return true;
    }
   if((node->val > imax) || (node->val <= imin))
   {
     return false;
   }
   if(!isBtreeBst(node->left, imin, node->val) || 
     !isBtreeBst(node->right, node->val, imax))
   {
     return false;
   }

   return true;
}

int main()
{
    char x;
    Btree *head = NULL;
    Btree *last = NULL;
    int idcount=1;
    printf("\nEnter options \n""b- if Btree is BST\n ""m- mirror image check\n"
           "u - parent pointer traversal\n" "c - create node \n" "d - delete \n"
             "p-print\n" "s - search\n" "l - LCA\n" "a - create with array\n" "q - quit \n");
    while(1)
    {
      printf("\nEnter option value\n");
      scanf("%c", &x);
      if(x == 'q')
        break;
      switch(x)
      {
        case 'a':
        {
          //int array[] = {60,30,80,20,50,70,90,35,55,65,74,85,100,52,58,95};
          int array[] = {60,30,80,20,50,70,90};
          head = new Btree(array[0], idcount++);
          //for(int x = 1; x < 16; x++)
          for(int x = 1; x < 7; x++)
          {
            insert(head, array[x], idcount++);
          }
          break;
        }
        case 'h':
        {
          //int array[] = {60,30,80,20,50,70,90,35,55,65,74,85,100,52,58,95};
          //int array[] = {60,30,80,20,50,70,90};
          int myints[] = {10,20,30,5,15};
          head = new Btree(myints[0], idcount++);
          //for(int x = 1; x < 16; x++)
          for(int x = 1; x < 5; x++)
          {
            last = insertMaxHeapElem(head, myints[x], idcount++);
            printBFSOrder(head);
          }
          printf("head %u ---- last %u last elem id %u \n", head->val, last->val, last->id);
#if 0
          printBFSOrder(head);
          int popedVal;

          for(int x = 1; x < 5; x++)
          {
            printf("*****************\n");
            last = heapPop(head, last, popedVal);
            printf("poped element %u \n", popedVal);
            printf("head %u ---- last %u last elem id %u \n\n", head->val, last->val, last->id);
            printBFSOrder(head);
            printf("******END********\n\n");
          }
#endif
#if 0
          for(int x = 1; x < 5; x++)
          {
            last = insertMinHeapElem(head, myints[x], idcount++);
          }
          printf("head %u ---- last %u \n", head->val, last->val);
#endif
          break;
        }
        case 'b':
        {
          int imin = INT_MIN;
          int imax = INT_MAX;
          //isBtreeBst(head, imin, head->val);
          printf("isBtreeBST %s \n", isBtreeBst(head, imin, imax)?"true":"false");
          break;
        }
        case 'c':
        {
          int value = 0;
          printf("Enter node value\n");
          scanf("%d", &value);
          if(!head)
          {
            head = new Btree(value, idcount++);
          }
          else
          {
            insert(head, value, idcount++);
          }
        break;
        }
        case 'p':
        {
        cout << "Tree pretty print with level=1 and indentSpace=0\n\n";
        PrettyBinaryPrint pretty;
        pretty.printPretty(head, 1, 0, cout);
        printf("\n");

        printf("Pre Order tree structure is ---\n");
        printPreOrderTree(head);
        printf("\n");

        printf("In Order tree structure is ---\n");
        printInOrderTree(head);
        printf("\n");

        printf("Post Order tree structure is ---\n");
        printf("\n");
        postOrderTree(head);
        printf("\n");

        //printf("tree height is --- %d\n", getHeight(head));
        //cout <<endl;
        printBFSOrder(head);
        }
        break;
        case 'l':
        {
          int value1 = 0, value2 = 0;
          printf("\nEnter two tree node values to find out common ancestor \n");
          scanf("%u %u", &value1, &value2);
         // LeastCommonAncestor(head, value1 , value2);
        }
        break;
        case 'm':
        {
         printf("isMirrorImage %s\n", isMirrorImage(head->left, head->right)?"true":"false");
        }
        break;
        case 'u':
        {
          int value = 0;
          printf("Enter node value to be searched\n");
          scanf("%d", &value);
         // Btree *node1 = searchTree(head, value);
         // if(node1)
          //{
           // printf("value(%u) is present in tree\n\n", node1->GetValue());
         // }
          //traverseTillRootwithParentPtr(node1);
          break;
        }
        default:
        break;
      }
    }
    return 0;
}
