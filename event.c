#include"include/event.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

char* read_last_line(char *path);

event* event_get_said(char *path){
    
    char *last_line = read_last_line(path);
    event *ret = malloc(sizeof(event));
    char *tmp = last_line;
    short count = 0;

    strcpy(&ret->type,"said");
    ret->time = time(NULL);

    while (*tmp != 0x3e & *tmp != 0x00){
        tmp++;
        if(*tmp == 0x3c | count > 0){
            count++;
        }
    }
    if(*tmp == 0x00){
        return NULL;
    } 
    count -= 2;
    tmp -= count;
    memcpy(&ret->user,tmp,sizeof(char)*count);
    ret->user[count] = 0;

    count = 0;
    while (*tmp != 0x00){
        tmp++;
        if(*tmp == 0x3e | count > 0){
            count++;
        }
    } 
    count -= 3;
    tmp -= count;
    memcpy(&ret->data,tmp,sizeof(char)*count);
    ret->data[count] = 0;
    free(last_line);
    return ret;
} 






char* read_last_line(char *path){
    FILE *pfile = fopen(path,"r");
    int count = 0;
    char *buf = malloc(sizeof(char)*128);
    int ch;

    fseek(pfile, 0L, SEEK_END);
    long pos = ftell(pfile);
    pos--;
    do {
        fseek(pfile, --pos, SEEK_SET);
        ch = fgetc(pfile);
        count++;
    } while (ch != '\n' && pos > 0);
    

    
    if(!fgets(buf,count,pfile)){
        printf("get last line error");
        return NULL;
    }
    fclose(pfile);
    return buf;

}
