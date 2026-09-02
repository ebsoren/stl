#include <iostream>

/*

A threadpool's domain is the allocation of threads to clients which request them.
The threadpool is responsible for the memory and lifetime of the threads.
A client should be able to:
1. Request a thread
2. Use the thread
and should never need to create or destroy a thread.

The threadpool can be defined as a class which takes one argument num_threads, and manages said threads until the class is destroyed,
at which point the threads are destroyed as well.
The thread pool should wait for all owned threads to terminate within its destructor.
The thread pool should also be safe if multiple clients use it simultaneuosly.

*/

#include "threadsafe_deque.hpp" 

