#ifndef DYNARR_CHAR
#define DYNARR_CHAR

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct DYN_ARR_CHAR
{
    int len,size;
    char *arr;
    void (*insert_end)(struct DYN_ARR_CHAR *, char *,int);
    char (*query)(struct DYN_ARR_CHAR, int);
    struct DYN_ARR_CHAR (*init)(char[], int);
    void (*free)(struct DYN_ARR_CHAR);
    void (*delet_end)(struct DYN_ARR_CHAR *);
}dyn_arr_char;

dyn_arr_char init(char arr[],int size);
void dyn_arr_char_free(dyn_arr_char arr);
void insert_end(dyn_arr_char *arr,char *value,int str_size);
void delet_end(dyn_arr_char *arr);
char query(dyn_arr_char arr,int select);

#endif
