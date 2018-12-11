#include <stdio.h>
#include <iostream>
#include <fstream>
#define MARKER -1

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
void serialize(ofstream &file, TreeNode *node)
{
    if(node == NULL)
    {
      file<<MARKER;
      file<<" ";
      return;
    }
    file<<node->m_value;
    file<<" ";
    serialize(file, node->m_left);
    serialize(file, node->m_right);
}

void deserialize(ifstream &file, TreeNode *&node)
{
    int element = 0;
    if(file>>element && (element == MARKER ))
        return;
    printf("deserialize:element is %d\n", element);
    node = new TreeNode(element);
    deserialize(file, (node->m_left));
    deserialize(file, (node->m_right));
}

void printInOrder(TreeNode *&node)
{
    if(!node)
     return;

    printInOrder(node->m_left);
    printf("%d \t", node->m_value);
    printInOrder(node->m_right);
}

void printPreOrder(TreeNode *&node)
{
    if(!node)
     return;
    printf("%d \t", node->m_value);
    printPreOrder(node->m_left);
    printPreOrder(node->m_right);
}

TreeNode *LeastCommonAncestor(TreeNode *node, int n1, int n2)
{
    if(node == NULL)
      return NULL;
    if(node->m_value == n1 || node->m_value == n2)
      return node;
    while(node != NULL)
    {
      if(node->m_value > n1 && node->m_value > n2)
        node = node->m_left;
      else if(node->m_value < n1 && node->m_value < n2)
        node = node->m_right;    
      else
      break;
    }
    return node;
}

bool CheckIfBst(TreeNode *node)
{
    if(node == NULL)
      return false;

#if 1
   if(node->m_left == NULL && node->m_right == NULL)
      return false;

   if((node->m_left == NULL) &&(node->m_value < node->m_right->m_value)) 
      return true;

   if((node->m_right == NULL) &&(node->m_value > node->m_left->m_value)) 
      return true;
#endif
    if((node->m_left->m_value < node->m_value) && (node->m_value < node->m_right->m_value))
      return true;

    bool r1 = CheckIfBst(node->m_left);
    bool r2 = CheckIfBst(node->m_right);

    if(r1 && r2) return true;
    else
      return false;
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
 //printInOrder(root);
 printPreOrder(root);
 printf("\n");

 {
   ofstream file;
   file.open("serialize.txt", std::ofstream::out);
   if(!file.is_open())
    {
      cout<<"1:file is not open"<<endl;
      return false;
    } 

   serialize(file, root);
   file.close();
 }

 {
   TreeNode *tree = NULL;
   ifstream file;
   file.open("serialize.txt", ifstream::in);
   if(!file.is_open())
    {
      cout<<"2:file is not open"<<endl;
      return false;
    } 
   deserialize(file, tree);
   file.close();
   printf("IN order print\n");
   //printInOrder(root);
   printPreOrder(tree);
   printf("\n");
 }

 printf("Tree is BST (%s) \n", CheckIfBst(root)?"TRUE":"FALSE");

 //int n1=8,n2=14;
 int n1=10,n2=22;
 TreeNode *node = LeastCommonAncestor(root, n1,n2);
 printf("LCA of (%d-%d) is (%d)\n", n1,n2, node->m_value);

 return 0;
}
