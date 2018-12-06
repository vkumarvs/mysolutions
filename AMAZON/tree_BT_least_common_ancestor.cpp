#include <stdio.h>
#include <iostream>

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



void printInOrder(TreeNode *&node)
{
    if(!node)
     return;
    printInOrder(node->m_left);
    printf("%d \t", node->m_value);
    printInOrder(node->m_right);
}

//This method assumes that both numbers are present in Binary tree
TreeNode *LeastCommonAncestor(TreeNode *node, int n1, int n2)
{
    if(node == NULL)
      return NULL;
    if(node->m_value == n1 || node->m_value == n2)
      return node;

    TreeNode *left_lca = LeastCommonAncestor(node->m_left, n1,n2);
    TreeNode *right_lca = LeastCommonAncestor(node->m_right, n1,n2);
    
    if(left_lca && right_lca)
      return node;
    return (left_lca != NULL)?left_lca:right_lca;
}

bool CheckIfBst(TreeNode *node)
{
    if(node == NULL)
      return false;

   if(node->m_left == NULL && node->m_right == NULL)
      return false;
#if 1

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

 TreeNode *root = new TreeNode(1);
 root->m_left = new TreeNode(2);
 root->m_right = new TreeNode(3);
 root->m_left->m_left = new TreeNode(4);
 root->m_left->m_right = new TreeNode(5);
 root->m_right->m_left = new TreeNode(6);
 root->m_right->m_right = new TreeNode(7);

 printf("IN order print\n");
 printInOrder(root);
 printf("\n");

 printf("Tree is BST (%s) \n", CheckIfBst(root)?"TRUE":"FALSE");
 //int n1=8,n2=14;
 int n1=4,n2=9;
 TreeNode *node = LeastCommonAncestor(root, n1,n2);
 printf("LCA of (%d-%d) is (%d)\n", n1,n2, node->m_value);
 return 0;
}
