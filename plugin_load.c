#include "include/plugin_load.h"
#include "include/thread_pool.h"

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <pthread.h>

static void get_name(struct dirent *stdinfo, char **name)
{
    int size;
    for (unsigned short tmp = 0; tmp < stdinfo->d_reclen; tmp++)
    {
        if (stdinfo->d_name[tmp] == 0x00)
        {
            size = tmp;
            break;
        }
    }
    *name = malloc(sizeof(char) * size + 1);
    memcpy(*name, &stdinfo->d_name, sizeof(char) * size + 1);
}

int plugin_list_load(plugin_t **head, char *path)
{
    DIR *dir;
    struct dirent *stdinfo;
    plugin_t *plugins = NULL;

    char *name;

    if ((dir = opendir(path)) == 0)
    {
        printf("Path not exist\n");
        return -1;
    }
    while ((stdinfo = readdir(dir)))
    {
        switch (stdinfo->d_type)
        {
        case DT_REG: // 普通文件
            get_name(stdinfo, &name);
            plugin_load(&plugins, name);
            break;
        default:
            continue;
        }
    }

    closedir(dir); // 关闭目录指针

    (*head) = plugins;
    return 0;
}

int plugin_load(plugin_t **head, char *name)
{
    plugin_t *tmp = malloc(sizeof(plugin_t));
    if (tmp == NULL)
    {
        return -1;
    }
    tmp->name = name;
    tmp->next = NULL;
    if ((*head) == NULL)
    {
        tmp->id = 0;
        tmp->end = tmp;
        (*head) = tmp;
    }
    else
    {
        tmp->id = (*head)->end->id + 1;
        tmp->end = NULL;

        (*head)->end->next = tmp;
        (*head)->end = tmp;
    }

    char path[128] = "./plugins/";
    strcat((char *)&path, tmp->name);
    tmp->dllib = dlopen(path, RTLD_NOW);
    if (!tmp->dllib)
    {
        return -1;
    }

    return 0;
}

int plugin_list_unload(plugin_t *head)
{
    plugin_t *tmp;
    do
    {
        tmp = head;
        head = head->next;
        if (dlclose(tmp->dllib))
        {
            printf("%s\n", dlerror);
            return -1;
        }
        free(tmp->name);
        free(tmp);
    } while (head);
}

int plugin_unload(plugin_t *head, char *name)
{
    plugin_t *tmp;
    if (!strcmp(head->name, name))
    {
        goto start;
    }
    for (; head != NULL && strcmp(head->name, name); head = head->next)
    {
        if (!strcmp(head->next->name, name))
        {
            tmp = head;
        }
    }
start:
    tmp = head;
    head = head->next;
    if (dlclose(tmp->dllib))
    {
        printf("%s\n", dlerror);
        return -1;
    }
    free(tmp->name);
    free(tmp);

    return 0;
}

int plugin_func(plugin_t *head, thread_pool_t *tpool, char *name, char *func_name, void *args)
{

    if (head == NULL)
    {
        return -1;
    }

    for (; strcmp(head->name, name) && head != NULL; head = head->next)
        ;
    void *(*func)(void *) = dlsym(head->dllib, func_name);
    if (!func)
    {
        return -1;
    }
    if (thread_pool_add_work(tpool, func, args))
    {
        return -1;
    }

    return 0;
}

