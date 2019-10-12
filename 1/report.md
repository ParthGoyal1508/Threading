## Concurrent QuickSort  

**data.txt contains 10^4 numbers which are in random order**

  
1.  **Normal Quicksort**
> Its the normal quicksort.
- How to run:
> gcc 1c.c -o 1c.out;
> ./1c.out

2.  **Quicksort using child processes created using fork**

> It uses **shmget** (for shared memory allocation) and **shmat** (for shared memory operations) functions. We create a shared memory space between the child process that we fork. Each segment is split into two part one carrying the elements greater than pivot and other carrying the elements less than pivot, the interesting part being they are working concurrently! The shmget() requests the kernel to allocate a shared page for both the processes. shmat() attaches the System V shared memory segment identified by **shmid** to the address space of the calling process.

 - How to run: 
> gcc 1a.c -o 1a.out;
> ./1a.out

 
- Why we need shared memory?
 
> The traditional fork does not work because the child process and the parent process run in separate memory spaces and memory writes performed by one of the processes do not affect the other. Hence we need a shared memory segment.

  

3.  **Quicksort using threading**

  > This implementation uses threads. There is one thread for every part containing less than the pivot element and other greater than the pivot elemet.
 - How to use:
> gcc 1b.c -pthread -o 1b.out;
> ./1b.out

  

**Time Analysis :**
| Cases | Real | User | Sys |
| ----------------------------- | -------- | -------- | -------- |
| Normal quicksort | 0m0.026 | 0m0.00s | 0m0.00s |
| Quicksort using forkprocesses | 0m1.799s | 0m4.92s | 0m2.08s |
| Quicksort using threads | 0m0.274s | 0m0.11s | 0m0.63s |
  

**Conclusion:**

> The time taken in **2nd case** is more than normal quicksort because when, (say left part) the array is loaded into the cache of a processor. Now when the right array is accessed (because of concurrent accesses), there is a cache miss since the cache is filled with left part and then right part is copied to the cache memory. This to-and-fro process continues and it degrades the performance to such a level that it performs poorer than the sequential code.

> The time taken in **3rd case** is more than 1st but less than the 2nd as we are creating large amount of threads each of which is doing very little work, therefore the overhead from creating all those threads far outweighs the gains you get from parallel processing. To speed this up we can make sure that each thread has a reasonable amount of work to do. For example, if one thread finds that it only has to sort <10000 numbers it can simply sort them itself with a normal quick sort, instead of spawning new threads. As you increase the number of threads beyond the number of cores on the system, you get less and less benefit from the multiple threads.