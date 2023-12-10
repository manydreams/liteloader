#include"include/plugin_load.h"
#include"include/thread_pool.h"

#include<pthread.h>

plugin* plugin_init_all(char *path,thread_pool_t *pool){
    plugin *head = plugin_list_load(path);
    open_all_plugin(path,head,pool);
    return head;
}



int main(int argc, char const *argv[])
{
    plugin *head = plugin_init_all(argv[1]);
    
    return 0;
}
