#ifndef PLUGIN_LOAD
#define PLUGIN_LOAD

typedef struct thread_pool thread_pool_t;

typedef unsigned long int pthread_t;

typedef struct plugin
{
    struct plugin *end;
    struct plugin *next;
    short id;
    char *name;
    void *dllib;
}plugin_t;



int plugin_list_load(plugin_t **head, char *path);

int plugin_load(plugin_t **head, char *name);

int plugin_list_unload(plugin_t *head);

int plugin_unload(plugin_t *head, char *name);

int plugin_func(plugin_t *head, thread_pool_t *tpool, char *name, char *func_name, void *args);

#endif