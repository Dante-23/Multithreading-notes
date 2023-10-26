#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

/*
    Example of creating a thread. 
*/

void thread1_create() {
    // Opaque object, don't bother about its internal members
    pthread_t pthread1;
    static char *input = "I am thread no 1";
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

}