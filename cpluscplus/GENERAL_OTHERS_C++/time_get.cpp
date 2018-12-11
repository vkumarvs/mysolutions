#include<iostream>
#include<stdio.h>
#include<sys/time.h>
using namespace std;
union UTime
{
    unsigned int time32;
    struct
    {
      unsigned hours : 5;
      unsigned minutes : 6;
      unsigned seconds : 6;
    };
};

int main()
{

    struct timeval tim;
    gettimeofday (&tim,NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
    sleep(1);             
    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
    printf("%.6lf seconds elapsed ......(%ld)\n" , t2-t1,tim.tv_sec);

#if 0
    UTime myTime;
    myTime.time32 = timerval.tv_sec;
    printf("seconds are (%u)\n",myTime.time32);
    printf("mytime in hour (%d) mytime in minutes(%d) mytimes in seconds(%d) \n",myTime.hours,myTime.minutes,myTime.seconds);
#endif
    return 0;
}

