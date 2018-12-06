#include <iostream>
#include <stdio.h>
#include <string>
#include <stdarg.h>
using namespace std;

#define LOGINFO(INFONUM, ...)                                        \
    if ((INFONUM) > 10) {                                   \
            CALLLOGERROR(INFONUM, __VA_ARGS__);                      \
    }

void CALLLOGERROR(int infonum, const char *format, ...)
        __attribute__ ((format (printf, 2, 3)));

inline void CALLLOGERROR(int infonum, const char *format, ...) {
    va_list ap;
           int d;
           char c, *s;
    va_start(ap, format);
           while (*format)
               switch (*format++) {
               case 's':              /* string */
                   s = va_arg(ap, char *);
                   printf("string %s\n", s);
                   break;
               case 'd':              /* int */
                   d = va_arg(ap, int);
                   printf("int %d\n", d);
                   break;
               case 'c':              /* char */
                   /* need a cast here since va_arg only
                      takes fully promoted types */
                   c = (char) va_arg(ap, int);
                   printf("char %c\n", c);
                   break;
               }
    va_end(ap);
}

string FillString()
{
    printf("I need to calculate string\n");
    return "My name is vipin";
}
#if 0
void returnstring(int x, string t)
{
 if(x == 10)
 {
   printf("Going to print everything (%s-%s)\n", t.c_str());
 }
}
#endif
int main()
{
    int x = 0;
    LOGINFO(x, "my string %s \n", FillString().c_str());
    x= 11;
    LOGINFO(x, "1:my string %s \n", FillString().c_str());
    return 0;
}

