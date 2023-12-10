#ifndef EVENT
#define EVENT

#include"plugin_load.h"
typedef struct event
{
    int time;
    char user[32];
    char data[128];

}event_t;

typedef struct event_broadcast
{
    char *type;
    event_t *event;
    plugin_t *reg_list;
    struct event_broadcast *next;
    struct event_broadcast *end;
}event_broadcast_t;

event_t* event_get_said(char *path);

int event_register(event_broadcast_t ** head, const char * type, plugin_t * plg);


#endif