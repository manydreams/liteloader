#include "include/event.h"
#include "include/thread_pool.h"

#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <dlfcn.h>

char *read_last_line(char *path)
{
    FILE *pfile = fopen(path, "r");
    int count = 0;
    char *buf = malloc(sizeof(char) * 128);
    int ch;

    fseek(pfile, 0L, SEEK_END);
    long pos = ftell(pfile);
    pos--;
    do
    {
        fseek(pfile, --pos, SEEK_SET);
        ch = fgetc(pfile);
        count++;
    } while (ch != '\n' && pos > 0);

    if (!fgets(buf, count, pfile))
    {
        printf("get last line error");
        return NULL;
    }
    fclose(pfile);
    return buf;
}

event_t *event_get_said(char *path)
{

    char *last_line = read_last_line(path);
    event_t *ret = malloc(sizeof(event_t));
    char *tmp = last_line;
    short count = 0;
    ret->time = time(NULL);
    while (*tmp != 0x3e & *tmp != 0x00)
    {
        tmp++;
        if (*tmp == 0x3c | count > 0)
        {
            count++;
        }
    }
    if (*tmp == 0x00)
    {
        return NULL;
    }
    count -= 2;
    tmp -= count;
    memcpy(&ret->user, tmp, sizeof(char) * count);
    ret->user[count] = 0;

    count = 0;
    while (*tmp != 0x00)
    {
        tmp++;
        if (*tmp == 0x3e | count > 0)
        {
            count++;
        }
    }
    count -= 3;
    tmp -= count;
    memcpy(&ret->data, tmp, sizeof(char) * count);
    ret->data[count] = 0;
    free(last_line);
    return ret;
}

int event_broadcast_call(event_broadcast_t *head, thread_pool_t *pool)
{
    if (!head)
    {
        return -1;
    }
    plugin_t *tmp;
    for (; head; head = head->next)
    {
        if (!head->event)
        {
            continue;
        }
        tmp = head->reg_list;
        for (; tmp; tmp = tmp->next)
        {
            void *(*func)(void *) = dlsym(tmp->dllib, head->type);
            if (!func)
            {
                return -1;
            }
            if (thread_pool_add_work(pool, func, head->event))
            {
                return -1;
            }
        }
    }
    return 0;
}

int event_broadcast_get(event_broadcast_t *head, thread_pool_t *pool)
{
    if (!head)
    {
        return -1;
    }
    for (; head; head = head->next)
    {
        head->event = malloc(sizeof(event_t));
        if (!head->event)
        {
            return -1;
        }
        head->event->time = 0;
        if (thread_pool_add_work(pool, head->event_get, (void *)head->event))
        {
            return -1;
        }
        usleep(0.0001);
        if (!head->event->time)
        {
            free(head->event);
            head->event = NULL;
        }
    }
    return 0;
}

int event_register(event_broadcast_t **head, const char *type, plugin_t *plg, void *(*event_get)(void *))
{
    if (!event_get)
    {
        return -1;
    }
    event_broadcast_t *tmp;
    char flag = 0;
    if (*head)
    {
        flag = 1;
        tmp = *head;
        if (!strcmp(tmp->type, type))
        {
        join:
            tmp->reg_list->end->next = malloc(sizeof(plugin_t));
            memcpy(tmp->reg_list->end->next, plg, sizeof(plugin_t));
            tmp->reg_list->end->next->end = NULL;
            tmp->reg_list->end->next->next = NULL;
            tmp->reg_list->end = tmp->reg_list->end->next;
            return 0;
        }

        for (; tmp->next && strcmp(tmp->next->type, type); tmp = tmp->next){}
        if (tmp->next)
        {
            tmp = tmp->next;
            goto join;
        }
    }

    tmp = malloc(sizeof(event_broadcast_t));
    if (!tmp)
    {
        return -1;
    }
    tmp->reg_list = malloc(sizeof(plugin_t));
    if (!tmp->reg_list)
    {
        free(tmp);
        return -1;
    }
    tmp->type = malloc(sizeof(char) * 64);
    if (!tmp->type)
    {
        free(tmp->reg_list);
        free(tmp);
        return -1;
    }
    tmp->event = NULL;
    memcpy(tmp->reg_list, plg, sizeof(plugin_t));
    tmp->reg_list->next = NULL;
    tmp->reg_list->end = tmp->reg_list;

    strcpy(tmp->type, type);
    tmp->next = NULL;
    tmp->event_get = event_get;
    if (flag)
    {
        (*head)->end->next = tmp;
        (*head)->end = tmp;
    }
    else
    {
        tmp->end = tmp;
        *head = tmp;
    }

    return 0;
}

int event_deregister(event_broadcast_t **phead,char *type,char *plugin_name){

    event_broadcast_t *head = *phead;
    event_broadcast_t *last_event = NULL;
    for (; head && strcmp(head->type, type); head = head->next){
        if(!strcmp(head->next->type,type)){
            last_event = head;
        }
    }
    if(!head){
        return -1;
    }
    
    plugin_t *plg = head->reg_list;
    plugin_t *last_plg = NULL;
    for(;plg && strcmp(plg->name,plugin_name); plg = plg->next){
        if(!strcmp(plg->next->name,plugin_name)){
            last_plg = plg;
        }
    }
    if(!plg){
        return -1;
    }
    if(last_plg){
        last_plg->next = plg->next;
        if(!last_plg->next){
            head->reg_list->end = last_plg;
        }
    }
    else{
        head->reg_list = plg->next;
        if(head->reg_list){
            head->reg_list->end = plg->end;
        }
    }
    free(plg->name);
    free(plg);

    if(head->reg_list){
        return 0;
    }

    if(last_event){
        last_event->next = head->next;
        if(!last_event->next){
            (*phead)->end = last_event;
        }
    }
    else{
        (*phead) = head->next;
        if(*phead){
            (*phead)->end = head->end;
        }
    }
    free(head->type);
    free(head);
    return 0;
}