#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

/*
    Example of creating a thread and joining. 
    Compiling :-
        g++ -g -c lecture2.cpp -o bin/lecture2.o
        g++ -g bin/lecture2.o -o bin/exe -lpthread

    When process needs to wait for IO while continue to execute :-
        * A process waiting for network packets in one threads while sending periodic packets
        in other threads while waiting to user input in other threads. 

    Threads share :-
        * Page tables.
        * Shared libraries.
        * Sockets etc.
    When a thread completes, kernel does not have to do a lot of cleanup.
    When thread switch happens, kernel again does not have to save a lot.
    Every thread have equal priority. Only main thread have this extra feature
    that if it is killed, all its child threads are killed.

    Joinable thread :-
        * When main thread waits for its child thread to complete (by calling pthread_join()), 
        then after child thread completes execution, then main thread is resumed and child thread's resources 
        are released. 
        * If main thread did not call join, then after child thread completion, its resources will not be
        cleared. 
        * A joinable thread can be converted to detacted while it is running and vice-versa. 
        * By default, thread runs on joinable mode. 
        * Joinable thread may return the result to joinee thread. 
        * When a thread terminates, all the threads which have called join on this thread resumes execution. 
        * Any thread can call join on any other thread. 
    Detached thread :-
        * Does not join main thread after completion. 
        * Resources of the detached thread are released as soon as thread terminates. 
        * Can be converted to joinable thread and vice versa. 
        * Does not return any value. 
    
    Map-reduce programming model :-
        * We have a big task to do. We divide the task among different worker threads. 
        * Each worker thread is known as mapper. 
        * The main thread which assigns work to worker thread is called reducer. 
        * Reducer waits for each mapper to complete its work. 
        * After every mapper completes its work, reducer resumes its execution. 
        * Example - counting number of words present in a very big text file. 
        * 
    
    Create thread as joinable when :-
        * It is supposed to return result. Eg - map-reduce.
        * When some thread needs to be notified about some other thread's termination.
    
    Create thread as detached when :-
        * No return result is expected.
        * Runs in infinite loop :-
        *   Waiting to user input.
        *   Waiting for network packets
        *   TCP server's worker thread interacting with TCP client. 
*/

void* thread_fn_callback(void *args) {
    int id = *(int*)args;
    free(args);
    int cnt = 0;
    while(cnt != id) {
        printf("Thread %d doing some work\n", id);
        sleep(1);
        cnt++;
    }
    int *result = (int*) calloc(1, sizeof(int));
    *result = id * id;
    return (void*)result;
}

void thread_create(pthread_t *handle, int id) {
    // attr to specify whether it is joinable or detached
    pthread_attr_t attr;
    // Initialize
    pthread_attr_init(&attr);
    int *heap_id = (int*) calloc(1, sizeof(int));
    *heap_id = id;
    // Make thread joinable by passing flag
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(handle, &attr, thread_fn_callback, (void*)heap_id);
}

int main() {
    pthread_t handle1, handle2;
    thread_create(&handle1, 2);
    thread_create(&handle2, 10);
    void *result, *result1;
    printf("Main thread blocked on join\n");
    // Wait for thread1 to complete. Main thread is blocked here. 
    // It will be unblocked once thread1 returns. 
    pthread_join(handle1, &result);
    // Main thread resumes execution. 
    if (result) {
        printf("Return result from 1st thread is %d\n", *(int*)result);
        free(result);
        result = NULL;
    }
    // Wait for thread2 to complete. Main thread is blocked here again. 
    // It will be unblocked once thread2 returns.
    pthread_join(handle2, &result1);
    if (result1) {
        printf("Return result from 2nd thread is %d\n", *(int*)result1);
        free(result1);
        result = NULL;
    }
    return 0;
}