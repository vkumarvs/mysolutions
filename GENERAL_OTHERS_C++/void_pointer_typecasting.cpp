#include <iostream>
using namespace std;
struct mystruct
{
    int a;
    int y;
};

int main()
{
    void* p_in = NULL;
    mystruct *x = (mystruct *)p_in;
    if(x == NULL)
      cout<<"i am null"<<endl;   
    return 0;
}
