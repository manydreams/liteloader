#ifndef EVENT
#define EVENT



typedef struct event
{
    int time;
    char type[32];
    char user[32];
    char data[128];
}event;


event* event_get_said(char *path);


#endif