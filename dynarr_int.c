#include"include/dynarr_int.h"


dyn_arr_int init(int arr[],int size){
    dyn_arr_int my_arr;
    my_arr.size = size;
    my_arr.len = size + 16;
    my_arr.arr = malloc(sizeof(int)*my_arr.len);
    memcpy(my_arr.arr,arr,sizeof(int)*size);
    my_arr.insert_end = &insert_end;
    my_arr.query = &query;
    my_arr.init = &init;
    my_arr.free = &dyn_arr_int_free;
    my_arr.delet_end = &delet_end;
    return my_arr;
}

void dyn_arr_int_free(dyn_arr_int arr){
    free(arr.arr);
}

void delet_end(dyn_arr_int *arr){
    if(arr->size==0){
        return;
    }
    (arr->size)--;
    if(arr->size<=arr->len-16){
        arr->len -= 16; 
        dyn_arr_int tmp;
        tmp.arr = malloc(sizeof(int)*(arr->len));
        memcpy(tmp.arr,arr->arr,sizeof(int)*arr->size);
        free(arr->arr);
        arr->arr = tmp.arr;
    }
}

dyn_arr_int insert_end(dyn_arr_int *arr,int value){
    if(arr->size==arr->len){
        arr->len += 16;
        dyn_arr_int tmp;
        tmp.arr = malloc(sizeof(int)*(arr->len));
        memcpy(tmp.arr,arr->arr,sizeof(int)*arr->size);
        free(arr->arr);
        arr->arr = tmp.arr;
    }
    arr->arr[arr->size++] = value;
}

int query(dyn_arr_int arr,int select){
    if(select>=arr.size){
        return 0;
    }
    int *p = arr.arr;
    return p[select];
}