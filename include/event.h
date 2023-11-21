#ifndef EVENT
#define EVENT

#define SAID 0
#define LEFT 1
#define JOINED 2

typedef struct event
{
    int time;
    unsigned char flag;
    char user[32];
    char date[128];
}event;


char* read_last_line(char *path);
int get_event_time(char *last_line);
event* get_event_said(char *last_line);
event* get_event_left(char *last_line);
event* get_event_joined(char *last_line);


#endif