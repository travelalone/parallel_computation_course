# Pthread

## Shared-memory Programming

- Defination: Process communicate or work together with each other through a shared memory space which can be accessed by all processes (faster and more efficient than message passing)

- Issue: Synchronization, deadlock, cache coherence

- Programming techniques: parallelizing compiler, unix processes, threads(Pthread, Java)

- Thread vs. Processes
  - Process(heavyweight process) complete separate program with its own variables, stack, heap, and everything else
  - thread(lightweight process) share the same memory space for global variables, resources
  - Linux(threads are created via clone a process with a flag to indicate the level of sharing)

- Why choosing Thread
  1. Thread is lightweight process. when using fork(), creating thread is much faster
  2. faster inter-process communication vs. MPI
  
## Pthread

- History
  - POSIX(Potable Operating System interface) standard is specified for portability across unix-like systems.
  - Pthread is the implementation of POSIX standard for thread

- API

- `pthread_create(thread, attr, routine, arg)`
  - thread: An unique identifier(token) for the new thread
  - attr: it is used to set thread attributes. NULL for the default values (could be null)
  - routine: the routine that the thread will execute once it is created
  - arg: A single argument that may be passed to routine
- `pthread_join(thread, *status)`
  - blocks until the specified threadId thread terminates
  - one way to accomplish synchronization between threads
- `pthread_detach(threadId)`
  - Once a thread is detached, it can never be joined
  - detach a thread could free some system resources

## Synchronization Problem and Tools

- The outcome of data content should not be decided by the execution order among processes
- instructions of individual processes/threads may be interleaved in time
- Critical Section is a piece of code that can only be accessed by one process/thread at a time
- Mutual exclusion is the problem to insure only one process/thread can be in a critical seletion  

How to solve?

- Lock: the simplest mechanism for ensuring mutual exclusion of critical section
- Locks are implemented in Pthreads by a special type of variable mutex
- To use mutex, it must be declared as of type `pthread_mutex_t` and intialized with `pthread_mutex_init()`
- a mutex is destroyed with pthread_mutex_destroy()
- a critical section can be protected using `pthread_mutex_lock()` and `pthread_mutex_unlock()`

```C
#include "pthread.h"
pthread_mutex_t mutex
pthread_mutex_init(&mutex, NULL)
pthread_mutex_lock(&mutex);

critical section

pthread_mutex_unlock(&mutex)
pthread_mutex_destroy(&mutex)
```

### Bounded-Buffer Problem

- A pool of n buffers, each capable of hold one item, producer process produce information that is consumed by a Consumer process
- Producer:
  - grab an empty buffer
  - place an item into the buffer
  - waits if no empty buffer is available
- Consumer:
  - grab a buffer and retracts the item
  - place the buffer back to the free pool
  - waits if all buffers are empty
- Buffer as a circular array with size B

```c
    /*producer*/
    while(1) {
        while(((in+1)%BUFFER_SIZE)==out);
        buffer[in] = nextProduced;
        in = (in + 1) % BUFFER_SIZE;
    }

    /*consumer*/
    while(1) {
        while(in == out);
        nextConsumed = buffer[out];
        out = (out+1) % BUFFER_SIZE
    }
```

### Condition Variable(CV)

- CV represent some condition that a thread can:
  - wait on, until the condition occurs
  - Notify other waiting threads that the condition has occured
- Tree opreations on condition variables
  - `wait()` Block until another thread call `signal()` or `broadcast()` on the CV
  - `signal()` wake up one thread waiting on the cv
  - `broadcast()` wake up all threads waiting on the cv
- In Pthread, CV type is `pthread_cond_t`

Example

- A threads is designed to take action when x = 0
- Another threads is responsible for decrementing the counter

```C
pthread_cond_t cond;
pthread_mutex_t mutex;
pthread_cond_init(cond, NULL);
pthread_mutex_init(mutex, NULL);

action(){
    pthread_mutex_lock(&mutex)
    if(x!=0)
        pthread_cond_wait(cond, mutex)
    pthread_mutex_unlock(&mutex)
    take_action();
}

counter(){
    pthread_mutex_lock(&mutex)
    x--;
    if(x==0)
        pthread_cond_signal(cond);
    pthread_mutex_unlock(&mutex);
}
```

All condition variable operation must be performed while a mutex is locked

why need lock?

- "x" is a shared variable
  - if no lock on thread `action()`, wait after any thread set "x" to 0;
  - if no lock on thread `counter()`

- `action`
  1. Lock mutex
  2. Wait()
     1. put the thread into sleep & released the lock
     2. waked up, but the thread is locked
     3. Re-acquire the lock and resume execution
- `counter()`
  1. Lock mutex
  2. Signal()
  3. Releases the lock

- Thread Pools
  - create a number of threads in a poll where they await work
  - advantages
    - Usually slightly faster to service a request with an existing thread than create a new thread
    - allows the number of threads in the application(s) to be bound to the size of the pool
  - number of threads: number of cpus, expected number of request, amount of physical memory

- ThreadPoll implementation
  - Task structure

   ```C
        typedef struct {
            void (*function) (void *);
            void *argument
        } threadpool_task_t;
   ```

  - Threadpool structure

    ```C
    struct threadpool_t{
        pthread_mutex_t lock;
        phtread_cond_t notify;
        pthread_t *threads;
        threadpool_task_t *queue;
        int thread_count;
        int queue_size;
        int head;
        int tail;
        int count;
        int shutdown;
        int started;
    }
    ```

  - allocate thread and task queue

    ```C
    pool *threads (pthread_t *) malloc(sizeof(pthread_t)) * thread_count);
    pool *queue (threadpool_tak_t *) malloc(sizeof(threadpool_task_t) * queue_size);
    ```

```C
    static void *threadpool_thread(void *threadpool)
    {
        threadpool_t *pool = (threadpool_t *) threadpool;
        threadpool_task_t task;
        for(;;){
            pthread_mutex_lock(&(pool->lock));
            while((pool->count==0) && (!pool->shutdown)){
                pthread_cond_wait(&(pool->notify), &(pool->lock));
            }
        }
    }
```

- Semaphore (control the number of accesses)
   1. A tool to generalize the synchronization problem
   2. a record of how many units of a particular resources are available
       - if number =1 binary semaphore
       - if number of record > 1 counting semaphore
       - accessed only through 2 atomic ops: `wait` and `signal`
       - semaphore is an integer variable
   3. semaphore is part of POSIX standard but it is not belong to Pthread

        ```C
        #include <semaphore.h>
        sem_t sem;
        sem_init(&sem);
        sem_wait(&sem);
        // do the work
        sem_post(&sem);
        sem_destry(&sem);
        ```

   4. all processes access a shared data object must excute `wait()` and `signal()` in the right order and right place

- Monitor
    High-Level synchronization construct that allows the safe sharing of an abstract data type among concurrent processes

- Synchronized Tools in JAVA
    1. Synchronized Methods(Monitor)
        - Synchronized method uses the method receiver as a lock
        - two invocations of synchronized methods cannot interleave on the same object
        - when one thread is excuting a synchronized method for an object, all other threads that invoke sychronized methods for the same object block until the first thread exist the object

        ```java
            public class SynchronizedCounter{
                private int c = 0;
                public synchronized void increment(){c++;}
                public synchronized void decrement(){c--;}
                public synchronized void int value(){return c;}
            }
        ```

    2. Synchronized Statement(Mutex Lock)
        - synchronized blocks uses the expression as a lock
        - a synchronized statement can only be excuted once the thread has obtained a lock for the object or the class that has been refered to in the statement
        - useful for improving concurrency with fine-grained

    ```java
    public void run()
    {
        synchronized(p1){
            int i = 0; 
            p1.display(s1);
        }
    }
    ```

### Big picture

- `Locks` are very simple for many cases, but may not be the most efficient solution
- `condition variables` allows threads to sleep while holding a lock
- `Semaphores` provide general functionality, but also make it really easy to mass up or cause deadlock
- `Monitors` are a pattern for using locks and condition variables