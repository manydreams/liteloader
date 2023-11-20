#include "include/plugin_load.h"

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <pthread.h>



int main()
{
    plugin *list;
    char *path = "./plugins/";
    list = plugin_list_load(path);
    if(open_all_plugin(path,list)){
        printf("failed");
    }
    // usleep(0.001);
    for(plugin *temp = list; temp != NULL; temp = temp->next){
        pthread_join(temp->thread,NULL);
    }
    return 0;
}


void get_name(struct dirent *stdinfo,char **name){
    int size;
    for(unsigned short tmp = 0; tmp < stdinfo->d_reclen; tmp++){
        if (stdinfo->d_name[tmp] == 0x00)
            {
                size = tmp;
                break;
            }    
        }
        *name = malloc(sizeof(char)*size+1);
        memcpy(*name,&stdinfo->d_name,sizeof(char)*size+1);
}

int open_all_plugin(char *path,plugin *head){

    for(plugin *temp = head; temp != NULL; temp = temp->next){
        char tmp[128];
        strcpy(tmp,path);
        strcat(tmp,temp->name);
        void (*init)(void*) = call_plugin_init(tmp);
        if(pthread_create(&head->thread,NULL,(void *)init,NULL)){
            printf("open %s failed\n",tmp);
            return 1;
        }
    }

    return 0;
}


int open_plugin(char *path,char *name,plugin *head){

    head = find_plugin_pointer(name,head);
    char tmp[128];
    strcpy(tmp,path);
    strcat(tmp,name);
    void (*init)(void*) = call_plugin_init(tmp);
    if(pthread_create(&head->thread,NULL,(void *)init,NULL)){
        printf("open %s failed\n",tmp);
        return 1;
    }
    return 0;
}

plugin* find_plugin_pointer(char *name,plugin *head){
    plugin* tmp;
    for(tmp = head; tmp != NULL; tmp = tmp->next){
        if(tmp->name == name) break;
    }
    if(tmp == NULL){
        printf("can't find %s\n",name);
        return NULL;
    }
    return tmp;
}

void* call_plugin_init(char path[]){
    void *plugin = dlopen(path,RTLD_LAZY);
    if(plugin == NULL){
        printf("dlopen failed : %s\n",dlerror());
        return NULL;
    }

    void (*init)(void) = dlsym(plugin,"init"); 
    if(init == NULL){
        printf("dlsym failed : %s\n",dlerror());
        return NULL;
    }
    return init;
}

void plugin_list_free(plugin *head){
    if(head != NULL){
        plugin_list_free(head->next);
        free(head);
    }
    return;
}

/// @brief load the plugins' list
/// @param path     --->path of plugins
/// @return pointer --->the plugins' list head pointer
plugin* plugin_list_load(char *path)
{
    DIR *dir;
    struct dirent *stdinfo;
    plugin *plugins = NULL;
    plugins = plugin_add(plugins,"hello.so");
    char *name;
    unsigned short size;

    if ((dir = opendir(path)) == 0){
        printf("Path not exist\n");
        return NULL;
    }
    while ((stdinfo = readdir(dir)))
    {
        switch (stdinfo->d_type)
        {
        case DT_REG: // 普通文件
            get_name(stdinfo,&name);
            plugin_add(plugins,name);
            break;
        default:
            // man readdir
            break;
        }
    }

    closedir(dir); // 关闭目录指针
    return plugins;
}

/*
If head is NULL , this func will create a plugin struct and the struct be used as head,
it will return head

Else , it will return NULL and let the head->next = new plugin's address
*/
plugin* plugin_add(plugin *head, char *name)
{

    if (head == NULL)
    {
        plugin *tmp = plugin_init();
        tmp->id = 0;
        tmp->name = name;
        tmp->flag = 1;
        tmp->head = tmp;
        tmp->end = tmp;
        return tmp;
    }

    plugin *tmp = plugin_init();
    tmp->id = head->end->id+1;
    tmp->name = name;
    tmp->last = head->end;
    tmp->flag = 2;
    tmp->head = head;
    tmp->end = tmp;
    head->end->next = tmp;
    head->end = tmp;
    return NULL;
}

plugin *plugin_init()
{
    plugin *plugin_list = malloc(sizeof(plugin));
    plugin_list->last = NULL;
    plugin_list->next = NULL;
    plugin_list->head = NULL;
    plugin_list->end = NULL;
    plugin_list->flag = 0;
    return plugin_list;
}
