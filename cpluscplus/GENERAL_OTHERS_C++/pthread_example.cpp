#if 0
void cleanup_after_malloc(void* allocated_memory)
{
    pthread_mutex_unlock
}



char * data = NULL;
pthread_cleanup_push(cleanup_after_malloc, (void *)data);                 
pthread_cleanup_pop(1);        
#endif
#include <stdio.h>
#include <pthread.h>
#include <iostream>

using namespace std;
void startLoad()
{
   printf("i am inside start load\n");
    while(true)
    {
      sleep(1);
    }
}

static void* ThreadEntry(void *ptr)
{
   printf("i am inside thread entry \n");
   startLoad(); 
}

int main()
{
    pthread_t m_thread;
    int ret = pthread_create(&m_thread, NULL, ThreadEntry, NULL);
    if(ret == 0)
    { 
      printf("Successfully created processing thread\n ");
    }                                                                             
    else
    { 
      printf("Error: cannot create Path Processing thread (error: %d)\n", ret);   
    }
   pthread_join(m_thread, NULL);
}

