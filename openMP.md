#OpenMP

## Introduction

- Format:

  | #pragma omp | directive-name                            | [clause,…]                                                   | newline  |
  | ----------- | ----------------------------------------- | ------------------------------------------------------------ | -------- |
  | Required    | Valid OpenMP directive: parallel, do, for | Optional, Clause can be in any order, and repeated at necessary | Required |

- Example:

  `#pragma omp parallel default(shared) private(beta, pi)`

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

- `DO/ for directive`

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