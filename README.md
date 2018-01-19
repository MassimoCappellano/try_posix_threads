# TRY POSIX THREADS

Started from [POSIX thread (pthread) libraries](http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html)  

[pthread_cond_timedwait manpage](http://man.yolinux.com/cgi-bin/man2html?cgi_command=pthread_cond_timedwait)

## USE of `pthread_cond_timedwait`

[`cond_timedwait.c`](cond_timedwait.c) some proof with `pthread_cond_signal` and `pthread_cond_timedwait`.

Thead functionCount increment counter and wait for 5 secs using `pthread_cond_timedwait`. 
On `SIGINT` capture, the handler call `pthread_cond_signal` and the `pthread_cond_timedwait` in thead exit before timeout.

```c

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

```

Question [pthread_cond_timedwait()](https://stackoverflow.com/questions/1486833/pthread-cond-timedwait).  
Question [Calling pthread_cond_signal without locking mutex](https://stackoverflow.com/questions/4544234/calling-pthread-cond-signal-without-locking-mutex).








