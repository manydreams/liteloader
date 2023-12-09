#ifndef THREAD_POOL
#define THREAD_POOL

#include<pthread.h>

typedef struct thread_pool_works
{
    void *(*func)(void *);
    void *args;
    struct thread_pool_works *next;
} thread_pool_works_t;
typedef struct thread_pool
{
    int shutdown;
    int max_thread;
    pthread_t *thread_id;
    pthread_cond_t ready;
    pthread_mutex_t lock;
    thread_pool_works_t *works_head;
    thread_pool_works_t *works_end;
}thread_pool_t;


/// @brief create a thread pool
/// @param pool  a NULL pointer
/// @param max_thread_num  create max_thread_num threads at thread pool
/// @return -1 ---> error, 0  ---> ok 
int thread_pool_create(thread_pool_t **pool,int max_thread_num);


/// @brief delet a thread pool
/// @param pool  a thread pool pointer
/// @return -1 ---> error, 0  ---> ok 
int thread_pool_delet(thread_pool_t *pool);


/// @brief add a work to thread pool
/// @param pool a thread pool pointer
/// @param func add the function to thread pool
/// @param args the funcion args
/// @return -1 ---> error, 0  ---> ok
int thread_pool_add_work(thread_pool_t *pool,void* (*func)(void*),void* args);


/// @brief wait work list finish
/// @param pool a thread pool pointer
/// @return -1 ---> error, 0  ---> ok
int thread_pool_quit(thread_pool_t *pool);

#endif