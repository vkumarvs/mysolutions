#include <stdio.h>
#include <iostream>
#include <fstream>
#define MARKER -1
#include <stack>
using namespace std;

struct TreeNode
{
  TreeNode(int val)
  {
    m_value = val;
    m_left = NULL;
    m_right = NULL;
  }
  int m_value;
  TreeNode *m_left = NULL;
  TreeNode *m_right = NULL;
};

class BstIterator
{
public:
    BstIterator(TreeNode *node)
    {
      pushall(node);
    }

    void pushall(TreeNode *node)
    {
        while(node != NULL)
        {
          mystack.push(node);
          node = node->m_left;
        }
    }

    int next()
    {
     TreeNode *tmp =  mystack.top();
     mystack.pop();
     pushall(tmp->m_right);
     return tmp->m_value;
    }

    bool hasNext()
    {
      return !mystack.empty();
    }
private:
   stack<TreeNode *> mystack;
};


void createTree(TreeNode **node, int val)
{
    if(!*node)
    {
      *node = new TreeNode(val);  
      return;
    }
    TreeNode *tmp = *node;
    while(tmp != NULL)
    {
      if(tmp->m_value > val)
      {
        if(!tmp->m_left)
        {
          tmp->m_left = new TreeNode(val);  
          return;
        }
        else
        {
          tmp = tmp->m_left;
        }
      }
      else if(tmp->m_value < val)
      {
        if(!tmp->m_right)
        {
          tmp->m_right = new TreeNode(val);  
          return;
        }
        else
        {
          tmp = tmp->m_right;
        }
      }
    }
}
void printInOrder(TreeNode *&node)
{
    if(!node)
     return;

    printInOrder(node->m_left);
    printf("%d \t", node->m_value);
    printInOrder(node->m_right);
}

int main()
{
 int array[] = {20,8,22,4,12,10,14};
 TreeNode *root = NULL;
 for (int x = 0; x < sizeof(array)/sizeof(int); x++)
 {
   createTree(&root, array[x]);
 }

 printf("IN order print\n");
 printInOrder(root);
 printf("\n");

 printf("ITERATOR  print\n");
  BstIterator it(root);
  while(it.hasNext())
  {
    cout <<it.next() << "\t";
  }
  cout<<endl;
 return 0;
}
