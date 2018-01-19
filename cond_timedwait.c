#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h> 
#include <pthread.h>

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

void *functionCount1();
void *functionCount2();
int  count = 0;

#define COUNT_DONE  10
#define COUNT_HALT1  3
#define COUNT_HALT2  6

void wait(int timeInMs)
{
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, NULL);
    ts.tv_sec = time(NULL) + timeInMs / 1000;
    ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (timeInMs % 1000);
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= (1000 * 1000 * 1000);

    int n = pthread_cond_timedwait(&condition_var, &count_mutex, &ts);
    if (n == 0) {
        printf("SIGNALED!!!!\n");
    }
        
    else if (n == ETIMEDOUT) {
        printf("TIMEOUT REACHED!!!!\n");
    }
        
}

void main()
{
   pthread_t thread1, thread2;

   pthread_create( &thread1, NULL, &functionCount1, NULL);
   pthread_create( &thread2, NULL, &functionCount2, NULL);

   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL);

   printf("Final count: %d\n",count);

   exit(EXIT_SUCCESS);
}

// Write numbers 1-3 and 8-10 as permitted by functionCount2()

void *functionCount1()
{
      printf("STARTING functionCount1()\n");

      // Lock mutex and then wait for signal to relase mutex
      pthread_mutex_lock( &count_mutex );

      printf("BEFORE WAIT!!!\n");
      fflush(stdout);

      wait(5000); // wait for 5 sec or for signal
      
      printf("AFTER WAIT: Counter value functionCount1: %d\n",count);

      pthread_mutex_unlock( &count_mutex );

      return(NULL);
    
}

// Write numbers 4-7

void *functionCount2()
{
    printf("STARTING functionCount2()\n");
    for(;;)
    {
       pthread_mutex_lock( &count_mutex );

       if( count > COUNT_HALT2 )
       {
          // Condition of if statement has been met. 
          // Signal to free waiting thread by freeing the mutex.
          // Note: functionCount1() is now permitted to modify "count".
          printf("--------------> ********************** SENDING SIGNAL!!!!\n");
          pthread_cond_signal( &condition_var );

          pthread_mutex_unlock( &count_mutex );
          break;
       }
       else
       {
          count++;
          printf("Counter value functionCount2: %d\n",count);
          // sleep(2);
          wait(2 * 1000);
       }

       pthread_mutex_unlock( &count_mutex );

       if(count >= COUNT_DONE) return(NULL);
    }

}
