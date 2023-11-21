#ifndef DYNARR_INT
#define DYNARR_INT

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct DYN_ARR_INT
{
    int len,size;
    int *arr;
    struct DYN_ARR_INT (*insert_end)(struct DYN_ARR_INT *, int);
    int (*query)(struct DYN_ARR_INT, int);
    struct DYN_ARR_INT (*init)(int[], int);
    void (*free)(struct DYN_ARR_INT);
    void (*delet_end)(struct DYN_ARR_INT *);
}dyn_arr_int;

void dyn_arr_int_free(dyn_arr_int arr);
dyn_arr_int insert_end(dyn_arr_int *arr,int value);
void delet_end(dyn_arr_int *arr);
int query(dyn_arr_int arr,int select);

#endif
