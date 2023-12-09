#include "include/thread_pool.h"

#include <stdlib.h>

static void *thread_run(void *args)
{
    thread_pool_t *pool = (thread_pool_t *)args;
    thread_pool_works_t *work = NULL;
    while (1)
    {
        pthread_mutex_lock(&pool->lock);
        while (!pool->shutdown && !pool->works_head)
        {
            pthread_cond_wait(&pool->ready, &pool->lock);
            // pthread_mutex_unlock(&pool->lock);
            // continue;
        }

        if (pool->shutdown)
        {
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }

        work = pool->works_head;
        pool->works_head = pool->works_head->next;
        pthread_mutex_unlock(&pool->lock);
        work->func(work->args);
        free(work);
    }
    return NULL;
}

int thread_pool_create(thread_pool_t **pool, int max_thread_num)
{
    thread_pool_t *tmp = malloc(sizeof(thread_pool_t));

    tmp->max_thread = max_thread_num;
    tmp->shutdown = 0;
    tmp->works_head = NULL;
    tmp->works_end = NULL;

    if (pthread_mutex_init(&tmp->lock, NULL) != 0)
    {
        return -1;
    }

    if (pthread_cond_init(&tmp->ready, NULL) != 0)
    {
        return -1;
    }

    tmp->thread_id = malloc(sizeof(pthread_t) * max_thread_num);
    if (!tmp->thread_id)
    {
        return -1;
    }

    for (int i = 0; i < max_thread_num; i++)
    {
        if (pthread_create(&(tmp->thread_id[i]), NULL, &thread_run, (void *)tmp) != 0)
        {
            return -1;
        }
    }
    *pool = tmp;
    return 0;
}

int thread_pool_quit(thread_pool_t *pool)(
    
    if (!pool)
    {
        return -1;
    }
    
    
    while(pool->works_head != NULL){
        usleep(0.001);
    }
)

int thread_pool_delet(thread_pool_t *pool){
    if (!pool)
    {
        return 0;
    }
    pool->shutdown = 1;

    pthread_mutex_lock(&pool->lock);
    pthread_cond_broadcast(&pool->ready);
    pthread_mutex_unlock(&pool->lock);

    thread_pool_works_t *work;
    while (pool->works_head)
    {
        work = pool->works_head;
        pool->works_head = pool->works_head->next;
        free(work);
    }

    for (int i = 0; i < pool->max_thread-1; i++)
    {
        pthread_join(pool->thread_id[i],NULL);
    }
    free(pool->thread_id);

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->ready);
    free(pool);
    return 0;
}

int thread_pool_add_work(thread_pool_t *pool,void* (*func)(void*),void* args){
    thread_pool_works_t *work = malloc(sizeof(thread_pool_works_t));
    if (!work)
    {
        return -1;
    }
    work->args = args;
    work->func = func;
    work->next = NULL;
    pthread_mutex_lock(&pool->lock);
    if (!pool->works_head)
    {
        pool->works_head = work;
        pool->works_end = pool->works_head;
    }
    else{
        pool->works_end->next = work;
        pool->works_end = work;
    }
    pthread_cond_signal(&pool->ready);
    pthread_mutex_unlock(&pool->lock);
    
    return 0;
}