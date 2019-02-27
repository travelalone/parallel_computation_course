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

### Bounded-Buffer Problem