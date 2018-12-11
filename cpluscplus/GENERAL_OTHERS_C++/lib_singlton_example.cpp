#include <iostream>
#include <stdio.h>
#include <stdint>

using namespace std;

class lib_mgr
{
    int x;
    bool flag;

public:
    lib_mgr(){}
    lib_mgr(int x, bool y)
    {
    }
};

class app_mgr:public lib_mgr
{
    app_mgr()
    {
    }
public:
    static app_mgr & GetInstance()
    {
      static app_mgr _singleton;
      return _singleton;
    }

};

int main()
{
    return 0;
}
