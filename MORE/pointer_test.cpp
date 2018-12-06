#include <stdio.h>

using namespace std;

class Socket
{
public:
    Socket(int *t)
    {
       y = t;
    };

    void printValue()
    {
      printf("Value is %d \n", *y);
    }
private:
    int *y;
};

class InternalClientService
{
public:
    InternalClientService()
    {
      x = new int;
      *x = 10;
    }
    int * getInternalFD() const;
private:
   int *x ;
};

int * InternalClientService::getInternalFD() const { return x; }

int main()
{
    InternalClientService *obj = new InternalClientService();
    Socket *soc = new Socket(obj->getInternalFD());
    soc->printValue();
    return 0;
}
