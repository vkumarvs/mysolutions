#include <iostream>
#include <stdio.h>
class Simple2DPoint
{
    public:
        Simple2DPoint& operator= (const Simple2DPoint &simpleObject);
};

Simple2DPoint& Simple2DPoint::operator= (const Simple2DPoint &simpleObject) {
    std::cout << "Hello I am being printed" <<std::endl;
    return *this;
}

int main() {
    Simple2DPoint X,Y;
    X=Y;
    printf("address %p \n",  &X);
    std::cout << &X <<std::endl;
    return 0;
}
