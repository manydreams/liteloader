#include "include/event.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


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

int event_register(event_broadcast_t **head,const char *type,plugin_t *plg){
    
    event_broadcast_t *tmp;
    char flag = 0;
    if(*head ){
        flag = 1;
        tmp = *head;
        if (!strcmp(tmp->type,type))
        {
join:
            tmp->reg_list->end->next = malloc(sizeof(plugin_t));
            memcpy(tmp->reg_list->end->next,plg,sizeof(plugin_t));
            tmp->reg_list->end->next->end = NULL;
            tmp->reg_list->end->next->next = NULL;
            tmp->reg_list->end = tmp->reg_list->end->next;
        }
        
        for(; tmp->next && strcmp(tmp->next->type,type); tmp = tmp->next);
        if(tmp->next){
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
    memcpy(tmp->reg_list, plg, sizeof(plugin_t));
    strcpy(tmp->type, type);
    tmp->next = NULL;
    tmp->event = NULL;
    if(flag){
        (*head)->end->next = tmp;
        (*head)->end = tmp;
    }
    else{
        tmp->end = tmp;
        *head = tmp;
    }

    return 0;
}

