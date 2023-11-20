

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
    

}plugin;


void plugin_list_free(plugin *head);
plugin* plugin_list_load(char *path);
plugin* plugin_init();
plugin* plugin_add(plugin *head, char *name);
