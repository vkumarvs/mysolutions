#include <iostream>
#include <time.h>
#include <stdio.h>
#include <ctime>
using namespace std;



void getExpireTime() {
    std::time_t now= std::time(0);
    char buf[64];
    std::tm now_tm;
    std::strftime(buf, 42, "%a, %d-%b-%Y %X GMT", gmtime_r(&now, &now_tm));
    printf("time: %s \n", buf);
}

int main()
{
#if 0
    char buff[100];
    time_t now = time(NULL);
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    printf("time: %s \n", buff);

//time: 2016-10-17 16:02:35 
// Fri, 14 Oct 2016 12:24:41 GMT
#endif
    getExpireTime();
    return 0;
}
