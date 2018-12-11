

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
    int imin = INT_MIN;
    int imax = INT_MAX;
    printf("isBtreeBST %s \n", isBtreeBst(head, imin, imax)?"true":"false");
}

