# MPI

Message Passing Interface, it's an interface, not a library. Used for distribued memory system & high-performance computing

## Introduction

Goal:

- Portable: Run on different machines or platforms
- Scalabel: Run on million of compute nodes
- Flexible: MPI developers from MPI programmers

MPI Library:

- MPICH, implement on the ethernet
- MRPICH: implement on the infiniband

History:

- MPI implementations are a combination of MPI-1 and MPI-2 (2 add the io part)
- The MPI Forum is now drafting the MPI-3 standard

Programming Model

SPMD: single program multiple data

- Allow tasks to branch or conditionally execute only parts of the program(based on the id is not the same)
- MPI tasks of a parallel program can not be dynamically spawned during run time

Distributed memeory: MPI provide method of sending & receiving messages

## Communication methods

From the view of the pair of communicated process

    - Synchronous communication: sending and receiving data simultaneously
    - Asynchronous communication: sending and receiving data non-simultaneously

From the view of individual function

    - Blocking: has been used to describe routines that do not return until the transfer is completed
    - Non-blocking: has been used to describe routines that return whether or not the message has been received

Synchronous & blocking
    - Synchronous communication: commonly implemented by blocking call
    - Synchronous communication: transfer data and synchronize process

Synchronous/Blocking message Passing:
    - Sender: wait until the complete message can be accepted by the receiver before sending the message
    - receiver: wait until the message it is expecting arrives

Asynchronous/Non-Blocking Message passing
    - Message buffer: between source and desination to hold message( Buffer is provided by the library, `mpi_init()`)
    - Message buffer is a memory space at the sender and receiver side
    - For send routine, once the local actions has been completed and the message is safety on its way, the process can continuce with subsequent work

## MPI API

### Start

- header file: mpi.h
- mpi calls: return value is error code
- structure:
   1. #include "mpi.h"  
   2. MPI_Init()
   3. MPI_xxx
   4. MPI_Finalize()
- group:
  - groups define which collection of processes may commnicate with each oter
  - each group is associated with a communicator to perform its communcation function calls
  - `MPI_COMM_WORLD` is the pre-defined communicator for all processors
- Rank:
  - an unique identifer for each process in a communicator
  - assigned by the system when the process initalizes
  - Contiguous and begin at zero

### Calls

- `MPI_Init()`
  Initalizes the MPI execution environment
- `MPI_Finalize()`
  Terminate the MPI execution environment
- `MPI_Comm_size(comm, &size)`
    Determins the number of processes int the group associated with a communicator
- `MPI_Comm_rank(comm, &rank)`

    1. Determin the ramk of calling process within the communicator
    2. the rank is often referred to as a task ID