#include<stdio.h>
#include<pthread.h>

int init(void *argv){
    printf("ohhhhhhhh\n");
    pthread_t self = pthread_self();
    pthread_detach(self);
    pthread_exit(NULL);
}

