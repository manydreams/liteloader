#ifndef PLUGIN_LOAD
#define PLUGIN_LOAD

typedef unsigned long int pthread_t;

//if flag = 1,it's head
//if flag = 2,it's end
//default falg = 0
typedef struct plugin
{
    struct plugin *head;
    struct plugin *end;
    struct plugin *last;
    struct plugin *next;
    int flag;
    short id;
    char *name;
    pthread_t thread;

}plugin;



void plugin_list_free(plugin *head);
plugin* plugin_list_load(char *path);
plugin* plugin_init();
plugin* plugin_add(plugin *head, char *name);
void* call_plugin_init(char path[]);
plugin* find_plugin_pointer(char *name,plugin *head);
int open_all_plugin(char *path,plugin *head);
int open_plugin(char *path,char *name,plugin *head);

#endif