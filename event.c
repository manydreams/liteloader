#include"include/event.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>



void main(int argc, char *argv[]){
    // char *str;
    // str = read_last_line("");
    // printf("%s\n",str);

    // FILE *fp = NULL;
    // fp = fopen("./test.txt", "r");
    // char str[128];
    // rewind(fp);
    // fgets(str,128,fp);
    // printf("%s\n",str);
    // fclose(fp);
    event *tmp;
    char *str;
    str = read_last_line(argv[1]);

    // *str = *str<<10;
    printf("%s\n",str);

    if((tmp = get_event_said(str)) == NULL){
        printf("err\n");
    }
    get_event_left(str);
    get_event_joined(str);
    printf("123\n");
}

event* get_event_joined(char *last_line){

    if (last_line[26] == 0x45 | last_line[26] == 0x57)
    {
        return NULL;
    }

    event *ret;
    char *tmp = last_line+26;
    short count = 0;
    unsigned flag = 0;

    while (*tmp != 0x00 & flag < 2){
        *tmp++;
        if(*tmp == 0x3a | count > 0){
            count++;
        }
        if(*tmp == 0x20){
            flag++;
        }
    }    
    if(!(tmp[1] == 0x6a)){
        return NULL;
    }
    count -= 3;
    tmp -= count;
    memcpy(&ret->user,tmp,sizeof(char)*count);
    ret->user[count] = 0;
    ret->flag = JOINED;
    ret->time = get_event_time(last_line);
    char tmp2[] = "joined";
    memcpy(ret->date,tmp2,sizeof(tmp2));
    return ret;
}

event* get_event_left(char *last_line){

    if (last_line[26] == 0x45 | last_line[26] == 0x57)
    {
        return NULL;
    }

    event *ret;
    char *tmp = last_line+26;
    short count = 0;
    unsigned flag = 0;

    while (*tmp != 0x00 & flag < 2){
        *tmp++;
        if(*tmp == 0x3a | count > 0){
            count++;
        }
        if(*tmp == 0x20){
            flag++;
        }
    }    
    if(!(tmp[1] == 0x6c)){
        return NULL;
    }
    count -= 3;
    tmp -= count;
    memcpy(&ret->user,tmp,sizeof(char)*count);
    ret->user[count] = 0;
    ret->flag = LEFT;
    ret->time = get_event_time(last_line);
    char tmp2[] = "left";
    memcpy(ret->date,tmp2,sizeof(tmp2));
    return ret;
}

event* get_event_said(char *last_line){
    
    if (last_line[26] == 0x45 | last_line[26] == 0x57)
    {
        return NULL;
    }
    
    
    event *ret;
    char *tmp = last_line;
    short count = 0;

    ret->flag = SAID;
    ret->time = get_event_time(last_line);

    while (*tmp != 0x3e & *tmp != 0x00){
        *tmp++;
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
        *tmp++;
        if(*tmp == 0x3e | count > 0){
            count++;
        }
    } 
    count -= 3;
    tmp -= count;
    memcpy(&ret->date,tmp,sizeof(char)*count);
    ret->date[count] = 0;
    free(last_line);
    return ret;
} 

int get_event_time(char *last_line){
    int time;
    time = last_line[8]-0x30;
    time += (last_line[7]-0x30)*10;
    time += (last_line[5]-0x30)*60;
    time += (last_line[4]-0x30)*60*10;
    time += (last_line[2]-0x30)*60*60;
    time += (last_line[1]-0x30)*60*60*10;
    return time;
}

char* read_last_line(char *path){
    FILE *pfile = fopen(path,"r");
    int count = 0;
    char *buf = malloc(sizeof(char)*128);
    int ch;

    fseek(pfile, 0L, SEEK_END);
    long pos = ftell(pfile);
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
