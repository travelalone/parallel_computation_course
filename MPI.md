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

#### Point-to-Point Communication Routines

- `MPI_Send(buffer, count, type, dest, tag, comm)`
- `MPI_ISend(buffer, count, type, dest, tag, comm, request)`
- `MPI_Recv(buffer, count, type, dest, tag, comm, status)`
- `MPI_Irecv(buffer, count, type, dest, tag, comm, request)`
  - `buffer` Address space that references the data to be sent or received
  - `type` : `MPI_CHAR`, `MPI_SHORT`, `MPI_INT`, `MPI_LONG`, `MPI_DOUBLE`
  - `count`: indicates the number of data elements of a particular type to sent or received
  - `comm`: indicates the communication context
  - `source/dest`: the rank(task ID) of the sender/receiver
  - `tag`: arbitrary non-negative integer assigned by the programmer to uniquely identify a message. Send and receive operations must match message tags. `MPI_ANY_TAG` is the wild card
  - `status`: status after operation
  - `request`: used by non-blocking send receive operations(check the progress is achieved or not)
- `MPI_Wait(request, status)`: blocks until the operation has actually completed
- `MPI_Test()`: returns with a flag set indicating whether operation completed at that time

#### Collective Communication Routines

- `MPI_Barrier(comm)`: creates a barrier synchronization in a group, blocks until tasks in the group reach the same `MPI_Barrier` call

- `MPI_Bcast(&buffer, count, datatype, root, comm)`
    Broadcasts(sends) a message from the process with rank "root" to all other processes in the group
- `MPI_Scatter`(&sendbuf, sendcnt, sendtype, &recvbuf, recvcnt, recvtype, root, comm)
    Distributes distinct messages from a source task to all tasks
- `MPI_Gather(&senfbuf, sendcnt, sendtype, &recvbuf, recvcnt, recvtype, root, comm)`
    Gather distinct messages from each task in the group to a single destination task, is the reverse operation of `MPI_Scatter`
- `MPI_Reduce(&sendbuf, &recvbuf, count, datatype, op, dest, comm)`
    Applies a reduction operation on all tasks in the group and places the result in one task, Pre-defined Redcution Operations : MPI_MAX, MPI_MIN, MPI_SUM, MPI_PROD, MPI_LAND, MPI_LOR ...
- `MPI_Allgather(&sendbuf, sendcount, sendtype, &recvbuf, recvcount, recvtype, comm)`
    Concatenation of data to all tasks, this is equivalent to an `MPI_Gather` followed by an `MPI_Bcast`
- `MPI_Allreduce(&senfbuf, &recvbuf, count, datatype, op, comm)`
    Applies a reduction operation and places the result in all tasks

#### Group and Communicator Routines

- `MPI_Comm_Group(Comm, &Group)`
    Access the group associated with a given communicator
- `MPI_Group_incl(Group, size, ranks[], &NewGroup)`
    produce a group by including a subset of members from an existing group
- `MPI_Comm_create(Comm, NewGroup, &NewComm)`
    create a new communicator, the new commnuicator must be a subset of original group

### MPI_I/O

`MPI_File_open()` (collective call)

- file is opened only once in a collective manner
- MPI library will share and synchronize with each other to use the same file handler
- can handle both read and write together

Collective I/O (`MPI_File_read/write_all(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status)`)

- read/write to a shared memory buffer, then issue one file request
- reduce number of I/O request (good for small I/O)
- require synchronization

Independent I/O (`MPI_File_read/write`)

- read/write individually
- prevent synchronization
- one request per process
- request is serialized if access the same OST
- Good for large I/O

API

- MPI_File_open(MPI_Comm comm, char *filename, int amode, MPI_Info info, MPI_File *fh)
- MPI_File_close(MPI_File *fh)
- MPI_File_sync(MPI_File Fh)