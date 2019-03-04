# OpenMP

## Introduction

- Format:

  | #pragma omp | directive-name                            | [clause,…]                                                   | newline  |
  | ----------- | ----------------------------------------- | ------------------------------------------------------------ | -------- |
  | Required    | Valid OpenMP directive: parallel, do, for | Optional, Clause can be in any order, and repeated at necessary | Required |

- Example:

  `#pragma omp parallel default(shared) private(beta, pi)`rules

- rules

  - Case sensitive
  - Only one directive-name may be specified per directive
  - Each directive applies to at most one succeeding statement, which must be a structured block

## Parallel Region Construct (parallel directive)

A parallel region is a block of code executed by multiple threads

```C
#pragma omp parallel [caluse ...]
	if(scalar_expression)
      num_threads(integer-expression)
structured_block
```

- Overview
  - When PARALLEL is reached, a team of threads is created
  - The parallel region code is duplicated and executed by all threads
  - There is an implied barrier at the end of a parallel section
  - One thread terminates, all threads terminate

- Limitations

  - A parallel region must be a  structured block that does not span multiple routines or code files
  - It is illegal to branch (goto) into or out of a parallel region, but you could call other functions within a parallel region (not suggest)

- The number of threads in a parallel region is determined in order of following precedence:

  - Evaluation of the IF cluase
    - If False, it is executed serially by the master thread
    - E.g: `#pragma omp parallel IF(para==true)`
  - Setting of the `num_threads` clause
    - E.g: `pragma omp parallel num_threads(10)`
  - Use of `omp_set_num_threads()` library function
    - Called before the parallel region
  - Setting of the `OMP_NUM_THREADS` environment variable
    - Called before the parallel region
  - By default,  usually the number of CPUS on a node

- Nested parallel Region

  ```C
  #pragma omp parallel num_threads(2)
  {
      #pragma omp parallel num_threads(3)
      {
          printf("hello world!");
      }
  }
  ```

  - Check if nested parallel regions are enabled `omp_get_nested()`
  - To disable/enable nested parallel regions:
    - `omp_set_nested(bool)`
    - Setting of the `OMP_NESTED` environment variable
  - If nested is not supported or enabled:
    - Only one thread is created for the nested parallel region code

## Working-Sharing Construct

- Definition:
  - A work-sharing construct divides the execution of the enclosed code region among the threads that encounter it
  - Working-sharing constructs do not launch new threads
  - There is no implied barrier upon entry to a work-sharing construct, however there is an implied barrier at the end of a work sharing construct

- type

  - **DO / for** (shared iterations of a loop across the team) Represents a type of "data parallelism"

  - **SECTIONS**, breaks work into separate, discrete sections of code. Each section is executed by a thread

  - **SINGLE** serialized a section of code by running with a single thread (E.g: IO)

  - Should be enclosed within a parallel region for parallelism


  ### DO/ for directive

  - Purpose: indicate the iterations of the loop immediately following it must be executed in parallel by the team of threads

    ```C
    #pragma omp for[clause...]
    	schedule[type, [,chunk]]
    	ordered
    	nowait
    	collapse(n)
    for_loop
    ```

  - Do/for Directive Specific Clauses:

    - `nowait`: Do not synchronize threads at the end of loop
    - `schedule`: describes how iterations are divided among threads
    - `ordered`: iterations must be executed as in a serial program (not suggest)
    - `collapse`: specifies how many loops in a *nested loop* should be collapsed into one large iteration space and divided according to the schedule clasue

  - `schedule`

    - `STATIC`
      - Loop iterations are divided into chunks (chuck could decrease the schedule time)
      - If chunk is not specified, the iterations are evenly (if possible) divided contiguously among the threads
      - Then statically assigned to threads
    - `DYNAMIC` when a thread finished one chunk (default size:1) it is dynamically assigned another (first come, first serve)
    - `GUIDED` similar to `DYNAMIC` except chunk size decreases over time (better load balancing)
    - `AUTO` the scheduling decision is delegated to the compiler and runtime system
    - `RUNTIME` the scheduling decision is deferred until runtime by the environment variable `OMP_SCHEDULE `(not suggest)

  - Collapse (increase the degree the parallelism, but notice the data dependence) 

    Generally, the `for` will only detect the first `for loop`, but when use parallelism, the nested for loop will be detected

    ```C
    #pragma omp parallel num_thread(6)
    #pragma omp for scheudle(dynamcis) collapse(2)
    
    for(int i = 0 ;i<3; i++)
        for(int j = 0; j<3; j++)
            printf("i=%d, j = %d, thread=%d\n", i, j, omp_get_thread_num());
    ```

    > i = 0, j = 0, thread=0
    >
    > i = 0, j = 2, thread=1
    >
    > i = 1, j = 0, thread=2
    >
    > i = 2, j = 0, thread=4
    >
    > i = 0, j = 1, thread=0
    >
    > i = 1, j = 2, thread=3
    >
    > i = 2, j = 2, thread=5
    >
    > i = 1, j = 1, thread=2
    >
    > i = 2, j = 1, thread=4


### Section Directive

- A non-iterative work-sharing construct

- It specifies that the enclosed sections of CODE are to be divided among the threads in team

- Independent SECTION directives are nested within a *SECTION* directive

- Each SECTION in **executed ONCE by ONE thread**

- The mapping between threads and sections is **decided by the library implementation**

  ```C
  #pragma omp sections [clause....]
  {
      #pragma omp section
      	structured_block
      #pragma omp section
          structured_block
  }
  ```

  ```C
  #include <omp.h>
  #include <stdio.h> 
  
  void main() {
      int N = 1000;
      int a[N], b[N], c[N], d[N];
  
      #pragma omp parallel num_threads(2) shared(a,b,c,d)
      {
          #pragma omp sections
          {
              #pragma omp section
              {
                  for(int i = 0; i<N; i++) c[i] = a[i] + b[i];
              }
              #pragma omp section
              {
                  for (int i=0; i<N; i++) d[i] = a[i] + b[i];
              }
  
          }
      }
      printf("%d\n", c[0]);
      printf("%d\n", d[0]);
  }
  ```


###  SINGLE Directive

- The SINGLE directive specifies that the enclosed code is to be executed by only one thread in the team
- May be useful when dealing with sections of code that are not thread safe(such as I/O)
- Threads in the team that do not execute the SINGLE directive, wait at the end of the enclosed code block, unless a **nowait** clause is specified

```C
#include<omp.h>
#include <stdio.h>

void main()
{
    int input;
    #pragma omp parallel num_threads(4) shared(input)
    {
        #pragma omp single
        {
            scanf("%d", &input);
        }
    printf("input is %d\n", input);
    }
}
```

## Synchronization Constructs

- For synchronization purpose among threads

  `#pragma omp [synchronization_directive] [clause ....] structured_block`

- Synchronization Directives

  - `master` only executed by the master thread
    - No implicit barrier at the end
    - More efficient than SINGLE directive
  - `critical` must be executed by one thread at a time, threads will be blocked until the critical section is clear
  - `barrier` blocked until all threads reach the call
  - `atomic` memory location must be updated atomically (provide a mini-critical section)

- LOCK OpenMP Routine
  - `void omp_init_lock(omp_lock_t *lock)` initializeds a lock associated with the lock variable
  - `void omp_destroy_lock(omp_lock_t *lock)` Disassociates the given lock variable from any locks
  - `void omp_set_lock(omp_lock_t *lock)` Force the thread to wait until the specified lock is available
  - `void omp_unset_lock(omp_lock_t *lock)` Releases the lock from the executing subroutine
  - `int omp_test_lock(omp_lock_t *lock)` Attemptes to set a lock, but dose not block if unavailable
- Advantage of using critical over lock:
  - No need to declare, initialize and destroy a lock
  - you always have explicit control over where your cirtical section ends
  - Less overhead with compiler assist

## Data Scope Attribute Clauses

It's critical to understand the scope of each data

- OpenMP is based on shared memory programming model

- Most variables are shared by default

- Global shraed variables:

  - File scope variables, static

- Private non-shared variables

  - Loop Index variables
  - stack variables in subroutines called from parallel regions

- Data scope can be explicitly defined by clauses...

  - PRIVATE, SHARED, FIRSTPRIVATE, LASTPRIVATE
  - DEFAULT, REDUCTION, COPYIN

- `PRIVATE(var_list)`

  Declares variables in its list to be private to each thread; variable value is **not initialized** and will **not be maintained outside the parallel region**

- `SHARED(var_list)`

  Declares variables in its list to be **shared among all threads**, by default, all variables in the work sharing region are shared except the loop iteration counter

- `FIRSTPRIVATE(var_list)`

  Same as `private` clause, but the **variable is INITIALIZED**, according to the value of their original objects prior to entry into the parallel region

- `LASTPRIVATE(var_list)`

  Same as `PRIVATE` clause, with a **copy from the LAST loop iteration or section to the original variable object**

- `DEFAULT(PRIVATE|FIRSTPRIVATE|SHARED|NONE)`

  Allows the user to specify a default scope for **ALL variables** in the parallel region

- `COPYIN(var_list)`

  **Assigning the same variable value based** on the instance from the master thread

- `COPYPRIVATE(var_list)`

  - Broadcast values acquired by a single thread directly to all instances in the other thread
  - Associated with the **SINGLE directive**

- `REDUCTION(operator: var_list)`

  - A private copy for each list variable is created for each thread
  - Performs a reduction on all variable instances
  - Write the final result to the global shared copy

## Run-Time Library Routines

- `void omp_set_num_threads(int num_threads)`

  Sets the number of threads that will be used in the next parallel region

- `int omp_get_num_threads(void)`

  Returns the number of threads currently executing for the parallel region

- `int omp_get_thread_num(void) `

  Returns the thread number of the thread, within the team, making this call, the master thread of the team is thread 0

- `int omp_get_thread_limit(void)`

  Returns the maximum number of OpenMP threads available to a program

- `int omp_get_num_procs(void)`

  Returns the number of processors that are available to the program

- `int omp_in_parallel(void)`

  determine if the section of code which is executing is parallel or not


