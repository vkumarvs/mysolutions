#include<stdio.h>
#include<stdlib.h>
typedef struct _binTree
{
  int data;
  struct _binTree *left;
  struct _binTree *right;

}BinTree;
//BinTree* addNode(BinTree *root,int x);
void addNode(BinTree **root,int x);
void addIterativeNode(BinTree **root,int x);
void travInOrder(BinTree *root, int *arr);
void PrintKeyBetweenRange(BinTree *root, int k1, int k2);

void ancestorPrint(BinTree *node, int key)
{

	if(node->data == key)
	{
		return;
	}
	else 
	{
		if(node->data > key)
		{
                      // printf("greater::looking for %d\n key %d\n", node->data, key);
			ancestorPrint(node->left, key);
				printf("%d --->",node->data);
		}
		else if(node->data < key)
		{
                       //printf("less::looking for %d\n key %d\n", node->data, key);
			ancestorPrint(node->right, key);
				printf("%d --->",node->data);
		}
	}
}
void performActionOnBinaryTree(BinTree **root)
{
  int x;
  printf("Choose Action you want to perform on binary tree\n 1:addNode \n 2:InTraverse\n 3:Ancestors Of a Key \n 4:quit \n5: KeyBetween Range\n");
  scanf("%d", &x);
  switch(x)
  {
	  case 1:
		  {
			  int l;
			  printf("Enter if any new element \n");
			  scanf("%d",&l);
			  addNode(root,l);
		  }
		  break;
	  case 2:
		  printf(" IN Order Traversing \n");
          int arr[10];
		  travInOrder(*root, arr);
          //printf("6th node in inorder traversal %d\n", arr[6]);
		  printf("\n");
		  break;
	  case 3:
               {
                  int key;
                  printf("Enter key for which ancestor need to be print\n");
                  scanf("%d", &key);
		  printf(" Ancestor printing of key %d  \n", key);
                  ancestorPrint(*root, key);
		  printf("\n\n");
                }
		  break;

	  case 4:
		  exit(1);
	  case 5:
               {
                  int k1,k2;
                  printf("Enter key range for which ancestor need to be print\n");
                  scanf("%d %d", &k1, &k2);
		  printf(" Ancestor printing of key1 %d key2%d \n", k1, k2);
                  PrintKeyBetweenRange(*root, k1, k2);
		  printf("\n\n");
                }
		  break;
	  default :
		  printf(" error in giving arguments \n");
  }
	performActionOnBinaryTree(root);
}

void createBinaryTree(BinTree **root)
{
	int i,y,l;
	printf("Enter how many element to enter  \n");
	scanf("%d",&y);
	for(i=0;i<y;i++)
	{  
		printf("Enter element   \n");
		scanf("%d",&l);
	//	addNode(root,l);
        addIterativeNode(root,l);
	}
	performActionOnBinaryTree(root);
}

int main()
{
  int y;
  BinTree *root = NULL;
  createBinaryTree(&root);
}

void addIterativeNode(BinTree **node,int x)
{
    BinTree *temp = *node;
    if(!(*node))
    {
      (*node) = malloc(sizeof(BinTree));
      (*node)->data = x;
      (*node)->left = NULL; 
      (*node)->right = NULL;
      return ; 
    }
    while(temp!=NULL)
    {
      if((temp)->data > x)
      {
        if((temp)->left !=NULL)
        {
          (temp) = (temp)->left;
        }
        else
        {
          (temp)->left = malloc(sizeof(BinTree));
          (temp)->left->data = x;
          (temp)->left->left = NULL; 
          (temp)->left->right = NULL;
          return ; 
        }
      }
      else if((temp)->data <= x)
      {
        if((temp)->right !=NULL)
        {
          (temp) = (temp)->right;
        }
        else
        {
          (temp)->right = malloc(sizeof(BinTree));
          (temp)->right->data = x;
          (temp)->right->left = NULL; 
          (temp)->right->right = NULL;
          return ; 
        }
      }
    }

    printf("temp is null\n");

}

//BinTree* addNode(BinTree *node,int x)
void addNode(BinTree** node,int x)
{
  
  if(!(*node))
   {
    (*node) = malloc(sizeof(BinTree));
    (*node)->data = x;
    (*node)->left = NULL; 
    (*node)->right = NULL;
    return ; 
   
  }
  else if((*node)->data > x)
   {
    //node->left =  addNode((node->left),x);
         addNode(&((*node)->left),x);
   }
  else if((*node)->data <= x)
   {
    //node->right = addNode(node->right,x);
      addNode(&((*node)->right),x);
   }
}

void travInOrder(BinTree *root, int *arr)
 {
  static x =0;
  if(!root)
  {
   return;
  }
  else 
  {
   travInOrder(root->left, arr);
   printf("%d --->",root->data);
   arr[x++] = root->data;
   travInOrder(root->right, arr);
  }
   
}

void PrintKeyBetweenRange(BinTree *root, int k1, int k2)
{
   /* base case */
   if ( NULL == root )
      return;
 
   /* Since the desired o/p is sorted, recurse for left subtree first
      If root->data is greater than k1, then only we can get o/p keys
      in left subtree */
   if ( k1 < root->data )
     PrintKeyBetweenRange(root->left, k1, k2);
 
   /* if root's data lies in range, then prints root's data */
   if ( k1 <= root->data && k2 >= root->data )
     printf("%d ", root->data );
 
  /* If root->data is smaller than k2, then only we can get o/p keys
      in right subtree */
   if ( k2 > root->data )
     PrintKeyBetweenRange(root->right, k1, k2);
}
