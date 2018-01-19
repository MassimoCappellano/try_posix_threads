#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h> 
#include <pthread.h>
#include  <signal.h>

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

void *functionCount();

int running = 1;
int  count = 0;

void  INThandler(int);

/**
 * param timeInMs - timeout in msec. 
 * 
 * return -1 if SIGNAL arrived;
 *         1 if timeout;
 *         0 else
 **/

int wait(int timeInMs)
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
        return -1;
    }
        
    else if (n == ETIMEDOUT) {
        printf("TIMEOUT REACHED!!!!\n");
        return 1;
    } else {
        return 0;
    }
        
}


void main()
{
   signal(SIGINT, INThandler);
   
   printf("PRESS Ctrl + C for stopping!!!\n");

   pthread_t thread1;

   pthread_create( &thread1, NULL, &functionCount, NULL);

   pthread_join( thread1, NULL);

   printf("Final count: %d\n",count);

   exit(EXIT_SUCCESS);
}

void  INThandler(int sig)
{
     signal(sig, SIG_IGN);
     
     pthread_mutex_lock(&count_mutex);

     // Condition of if statement has been met. 
     // Signal to free waiting thread by freeing the mutex.
     // Note: functionCount1() is now permitted to modify "count".
     printf("--------------> SENDING SIGNAL!!!!\n");

     pthread_cond_signal(&condition_var);

     pthread_mutex_unlock(&count_mutex);

}


// increment counter and stop at signal 

void *functionCount()
{
      printf("STARTING functionCount1()\n");
      
      while(running) {

        // Lock mutex and then wait for signal to relase mutex
        pthread_mutex_lock(&count_mutex);

        printf("BEFORE WAIT!!!\n");

        if( wait(5000)== -1 ){
            printf("SETTED RUNNING FALSE\n");
            running = 0;
        }

        count++;
        
        printf("AFTER WAIT: Counter value functionCount1: %d\n",count);

        pthread_mutex_unlock(&count_mutex);

      }

      return(NULL);
    
}




