#include <iostream>
#include <set>
#include<stdio.h>


class EventConsumer
{
    public:
    EventConsumer(int x, int y):x_(x),y_(y){}
    int x_;
    int y_;
};

int main() {

    std::set<EventConsumer *> old;
    std::set<EventConsumer *> newsoc;
    EventConsumer *myevent =  new EventConsumer(10,11);
    old.insert(myevent);
    printf("size of the set (%zu)\n", old.size());
    myevent->y_ = 20;
    old.insert(myevent);
    newsoc.insert(myevent);
    old = newsoc;
    printf("size of the set (%zu)\n", old.size());
    return 0;
}
