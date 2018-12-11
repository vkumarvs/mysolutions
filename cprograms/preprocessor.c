#include <stdio.h>

#define SKIP_SPACES(p, limit)  \
     do{ char *lim = (limit);         \
       while (p < lim) {            \
         if (*p++ != ' ') {         \
           p--; break; }}} while(0)

#define FOUR (2 + 2) x
 //    #define FOUR         (2    +    2)
  //   #define FOUR (2 /* two */ + 2)

//#define FOUR ( 2+2 )
//#define FOUR (2+2) 
//#define FOUR (2 * 2)
//#define FOUR(score,and,seven,years,ago) (2 + 2)
#define f(x) x x
int main()
{
   int FOUR();
   f (1
     #undef f
     #define f 2
     f)

   int x = 10;
   int *p = &x;
    if (*p != 0)
      SKIP_SPACES (p, x);
    else 
    {
    }
}
