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
        * 
    One very important observation :-
        * cout is not thread safe. The output could get interleaved.
        You could try to make cout thread safe.
        * printf is thread safe. There would be no interleaving. 


    A great answer from stack overflow :-
        printf("Guest %ld goes to the check-in receptionist.\n", my_rank);
        When you use printf, your string is formatted into an internal buffer and then output to the console in a single operation ("Guest 2 waits for check-in.").

        cout << "Guest " << guestNumber << " waits for check-in." << endl;
        When you use cout, your string is output to the console in multiple parts - "Guest", followed by guestNumber, followed by " waits for check-in.", followed by endl. This is because each call to the << operator takes place as if it were a separate function call (it returns a reference to the same cout object for the next call to use).

        So although writing to the console itself is thread-safe and atomic, in the cout case it's only atomic for each separate sub-string.

        The solution if you don't want to use printf would be to a) use a semaphore or other locking mechanism, or b) format the text before printing it using stringstream, and then output it as a single string.
*/

void *thread_fn_callback(void *args) {
    char *input = (char*) args;
    int cnt = 0;
    while(cnt < 10) {
        string output = "";
        output += "Input string: ";
        output += input;
        cout << "Input string: " << input << "\n";
        // printf("Input string: %s\n", input);
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
    // pause();
    pthread_exit(0);
    return 0;
}