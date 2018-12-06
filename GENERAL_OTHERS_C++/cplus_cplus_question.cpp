//look explanation::a
//http://www.velocityreviews.com/forums/t268090-re-why-empty-class-size-is-1-byte.html
#include <iostream>
#include <sys/socket.h>
using namespace std;
class X
{
virtual void function2();
};

class Y
{
int a;
public:
void function1();
};

void Y::function1()
{
}

enum myEnumType
{
    MAX_ENUM
};

int main()
{
int *x = NULL;
string ab;
sockaddr_storage ipSt;
std::cout << sizeof(X) << '\n';
std::cout << sizeof(Y) << '\n';
std::cout <<"size of default string object       --> " << sizeof(ab) << '\n';
std::cout <<"size of pointer on 64bit machine    --> " << sizeof(x) << '\n';
std::cout <<"size of enum    on 64bit machine    --> " << sizeof(myEnumType) << '\n';
std::cout <<"size of sockaddr_storage on machine --> " << sizeof(ipSt) << '\n';
return 0;
}

//produces an output of 1 and 4.

