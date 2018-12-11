#include<stdio.h>
typedef struct _vipin
{
   int x;
   struct _vipin  *next; 
}vipin;

typedef union 
{
  int x;
  char y;
  float z;
}uni;
int xvipin;
void vipin_read()
{
   printf("hi am in vipin read\n");
}
int main()
{
   int x=10,j,k,l,m;
   int arr[] = {10,20,30,45,67,56,74};
   int *n,*tt;
   tt = &arr[1];
   n = &arr[5];
   printf("pointer difference %d\n",n-tt);
   printf("%d\n",x);
   int z=5;
   uni s;
   vipin p;
   vipin t;
   p.x=90;
  // p.next->x = 10;
   z = z+10;
   s.x=30;
   for(j=2,k=1; k<12&&j<8; j++, k++)
     printf("vipin \n");

   switch(10)
    {
       case 12:
       case14:
       case 11:
      default:
       printf("hi \n");
    }
    long int dd;
    float ll;
    char ar;
    short int op;
    double xx;
    void (*VIPFPTR)();
    VIPFPTR = vipin_read;
    VIPFPTR();
   
    printf("int size(%d),float size(%d),long int size(%d),size char(%d) short int(%d),sizeof double(%d)\n",sizeof(x),sizeof(ll),sizeof(dd),sizeof(ar),sizeof(op),sizeof(xx));
static int dfg;
  printf("static and global default values (%d) , (%d) \n",dfg,xvipin);
return 0;
}
