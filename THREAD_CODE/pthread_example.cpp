#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <random>
#include <ctime>
#include <chrono>
using namespace std;

thread_local std::default_random_engine dre1;// each thread has its own  rand engine copy
int64_t generateUniqueID() {
#if 0
    std::time_t nowTime = std::time(NULL);
    int randomNumber = rand();
    return ((nowTime << 32) | (randomNumber & 0xffffffff));
#else
    //auto seed = std::chrono::system_clock::now().time_since_epoch().count();//seed
    //thread_local std::default_random_engine dre(seed);// each thread has its own  rand engine copy
    std::uniform_int_distribution<int> di;//distribution
    std::time_t nowTime = std::time(NULL);
    return ((nowTime << 32) | (di(dre1) & 0xffffffff));

#endif

}

#if 0
int rand_int(int low, int high) {
    static std::default_random_engine re;
    using Dist = std::uniform_int_distribution<int>;
    static Dist uid;
    return uid(re, Dist::param_type{low,high});
}
#endif

void startLoad(int threadID)
{
	while(true)
	{
	  printf("threadID (%d) -- Next Random number is (%" PRId64 ")\n", threadID, generateUniqueID());
	  sleep(1);
	}
}

static void* ThreadEntry1(void *ptr)
{
    auto seed1 = std::chrono::system_clock::now().time_since_epoch().count();//seed
    dre1.seed(seed1);
    int id = 0;
   startLoad(id);
   return NULL; 
}

static void* ThreadEntry2(void *ptr)
{
    int id = 1;
    auto seed1 = std::chrono::system_clock::now().time_since_epoch().count();//seed
    dre1.seed(seed1);
   startLoad(id);
   return NULL; 
}

static void* ThreadEntry3(void *ptr)
{
    int id = 2;

    auto seed1 = std::chrono::system_clock::now().time_since_epoch().count();//seed
    dre1.seed(seed1);

   startLoad(id);
   return NULL; 
}

int main()
{
    pthread_t m_thread1, m_thread2, m_thread3;
    srand(time(NULL));
    int ret = pthread_create(&m_thread1, NULL, ThreadEntry1, NULL);
    if(ret == 0) { 
      printf("Successfully created processing thread\n ");
    }

    ret = pthread_create(&m_thread2, NULL, ThreadEntry2, NULL);
    if(ret == 0) { 
      printf("Successfully created processing thread\n ");
    }

    ret = pthread_create(&m_thread3, NULL, ThreadEntry3, NULL);
    if(ret == 0) { 
      printf("Successfully created processing thread\n ");
    }                                                                            

   pthread_join(m_thread1, NULL);
   pthread_join(m_thread2, NULL);
   pthread_join(m_thread3, NULL);
}

