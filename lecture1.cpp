#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

/*
    Example of creating a thread. 
    Compiling :-
        g++ -g -c lecture1.cpp -o bin/lecture1.o
        g++ -g bin/lecture1.o -o bin/exe -lpthread

    Thread termination :-
        * Thread fn returns
        * pthread_exit(0)
        * Thread cancellation (if another thread kill this thread)
    Kernel do not schedule processes, it schedules thread. 
    Thread is a schedulable entity, not a prcoess.
    However, this rule is violated in certain error conditions :-
        * If a thread seg-fault, entire processes is terminated.
        * A signal is delivered per process, not per thread.
    
*/

void *thread_fn_callback(void *args) {
    char *input = (char*) args;
    int cnt = 0;
    while(cnt < 10) {
        cout << "input string: " << input << endl;
        if (cnt == 5) pthread_exit(0);
        cnt++;
        sleep(1);
    }
}

void thread1_create(const char* input) {
    // Opaque object, don't bother about its internal members
    pthread_t pthread1;
    // Return 0 on success. Onn error, returns an error code.
    // All pthread functions return -ve error code and they do not
    // set global 'errno' variable
    int rc = pthread_create(&pthread1, NULL, thread_fn_callback,
                (void*)input);
    if (rc != 0) {
        cout << "Unable to create thread." << endl;
        exit(0);
    }
}

int main() {
    thread1_create("Thread1");
    thread1_create("Thread2");
    // If main thread exits, then all its child threads are killed. 
    // But if we do pthread_exit(0) on the main thread, 
    // then the child threads can continue to stay alive. 
    pause();
}