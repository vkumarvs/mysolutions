#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

bool usingdoublePointer(int x, void** y = NULL);
class A
{
private:
    void *x;
public:
    A()
    {
      x = NULL;
    }
    void **GetPointer(){return &x;}
};
void exp(void *ptr)
{
    int *x = (int *)(ptr);
    *x = 100;
}
int main()
{
    int x = 20;
    A obj;
    void *l = NULL;
      printf("lllllllllllllllll\n");
    usingdoublePointer(x, obj.GetPointer());
    exp(*(obj.GetPointer()));
 //   usingdoublePointer(x);
    printf("value of x is %d pointer (%p) --- (%p) value i(%d)\n",x, *(obj.GetPointer()), obj.GetPointer(), *((int *)(*(obj.GetPointer()))));
    return 0;
}

bool usingdoublePointer(int x, void ** y)
{
    int k =10;
    //int *t = (int *)malloc(sizeof(int));
    *y = (int *)malloc(sizeof(int));
#if 0
    int **l = (int *)(y);
    *l  = (int *)malloc(sizeof(int));
    *l = 50;
    printf("value of l is %d\n", *l);
#endif
     //*(int *)(*y)  = 50; 
#if 0
    *t = 50;
      printf("ttttttttttttt\n");
    
    if(y)
    {
      printf("bbbbbbaaaaaa\n");
      *y = t;
    }
    else
    {
      printf("aaaaaa\n");
      //*y = t;
    }
#endif
}

