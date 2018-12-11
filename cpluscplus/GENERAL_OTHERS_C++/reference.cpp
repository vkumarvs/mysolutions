#include <iostream>
#include <stdio.h>

using namespace std;

class refer
{
public:
    refer();
    int x;
    int y;
};

refer::refer()
{
      x = 0;
      y = 0;
}

int main()
{
    refer obj1, obj2;
    obj2.y = 10;
    obj1 = obj2;
    printf("value of y :: obj1(%d) obj2(%d)\n", obj1.y,obj2.y );
    return 0;
}

