#include"include/plugin_load.h"

#include<pthread.h>

plugin* plugin_load_all(char *path){
    plugin *head = plugin_list_load(path);
    open_all_plugin(path,head);
    return head;
}

void* plugin_unload_all(plugin *head){
    plugin *tmp = head;
    while (tmp != NULL)
    {
        pthread_join(tmp->thread,NULL);
        tmp = tmp->next;
    }
    plugin_list_free(head);
    return;
}

int main(int argc, char const *argv[])
{
    plugin *head = plugin_load_all(argv[1]);
    plugin_unload_all(head);
    return 0;
}
