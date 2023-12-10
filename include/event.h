#ifndef EVENT
#define EVENT

#include"plugin_load.h"
typedef struct event
{
    int time;
    char user[32];
    char data[128];

}event_t;


/// @brief event_get(event_t *)
typedef struct event_broadcast
{
    char *type;
    event_t *event;
    void* (*event_get)(void*);
    plugin_t *reg_list;
    struct event_broadcast *next;
    struct event_broadcast *end;
}event_broadcast_t;

event_t* event_get_said(char *path);

int event_register(event_broadcast_t ** head, const char * type, plugin_t * plg,void* (*event_get)(void*));

int event_broadcast_get(event_broadcast_t *head,thread_pool_t *pool);

int event_broadcast_call(event_broadcast_t *head,thread_pool_t *pool);

int event_deregister(event_broadcast_t **phead,char *type,char *plugin_name);

#endif