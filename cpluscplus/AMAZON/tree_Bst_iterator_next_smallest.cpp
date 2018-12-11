/*
 * Approach 1 : Do an inorder traversal of the tree and store the entries in an
 * array with the current pointer set to the start of the array. hasNext checks
 * if the pointer is less than the size of the array. next() would return the
 * element at the current position incrementing the position by 1.
 * However, this has an additional space complexity of O(N) where N = number of
 * nodes in the tree.
 *
 * Approach 2 : Lets look at the version of this problem when the trees have a
 * back pointer. We can solve the problem without using additional space. When
 * you are on node N and are asked for next element, you obviously won’t go to
 * the left subtree as all the elements there are smaller than N. We would go to
 * the smallest number in the right subtree if the right subtree is not null. If
 * the right subtree is null, that means that we need to move up, and keep
 * moving up till we are coming from the right subtree.
 * Now we don’t have the back pointer in this case. So, we need something to
 * keep track of the path from root to the current node, so we can move to the
 * parent when needed. Do note that storing the path from root to the current
 * node only requires memory equivalent to the length of the path which is the
 * depth of the tree. Also, we can track the path using stack.
 */

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
}

class BSTIterator
{
public:
    stack<BstTree*> myStack;

    BSTIterator(BstTree *root) {
      pushAll(root);
    }

    // return whether we have a next smallest number
    bool hasNext() {
      return !myStack.empty();
    }

    // return the next smallest number
    int next() {
      BstTree *tmpNode = myStack.top();
      myStack.pop();
      pushAll(tmpNode->m_rightChild);
      return tmpNode->GetValue();
    }

private:
    void pushAll(BstTree *root) {
      while (root != NULL) {
        myStack.push(root);
        root = root->m_leftChild;
      }
    }
};

void printInOrderTree(BstTree *node)
{
    if(!node)
    {
      return ;
    }
    printInOrderTree(node->m_leftChild);
    printf("(%d) \t", node->GetValue());
    printInOrderTree(node->m_rightChild);
}

int main()
{
    char x;
    BstTree *head = NULL;
    printf("\nEnter options \n" "p-print\n" "a - create with array\n" "q - quit \n" "i-implement next smallest iterator");
    while(1)
    {
      printf("\nEnter option value\n");
      scanf("%c", &x);
      if(x == 'q')
        break;
      switch(x)
      {
        case 'p':
        {
          printf("tree structure is ---\n");
          printInOrderTree(head);
          printf("\n");
          break;
        }
        case 'a':
        {
          int array[] = {60,30,80,20,50,70,90,35,55,65,74,85,100,52,58,95};
          head = new BstTree(array[0]);
          for(int x = 1; x < 13; x++)
          {
            createTree(head, array[x]);
          }
          break;
        }
        case 'i':
        {
          BSTIterator it(head);
          while (it.hasNext()) cout << it.next() << " ";
          break;
        }
        default:
        break;
      }
    }

    return 0;
}
