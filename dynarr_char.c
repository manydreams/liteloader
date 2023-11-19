#include"include/dynarr_char.h"


dyn_arr_char init(char arr[],int size){
    dyn_arr_char my_arr;
    size++;
    my_arr.size = size;
    my_arr.len = size + 16;
    my_arr.arr = malloc(sizeof(char)*my_arr.len);
    memcpy(my_arr.arr,arr,sizeof(char)*size);
    my_arr.insert_end = &insert_end;
    my_arr.query = &query;
    my_arr.init = &init;
    my_arr.free = &dyn_arr_char_free;
    my_arr.delet_end = &delet_end;
    return my_arr;
}

void dyn_arr_char_free(dyn_arr_char arr){
    free(arr.arr);
}

void delet_end(dyn_arr_char *arr){
    if(arr->size==0){
        return;
    }
    arr->size--;
    arr->arr[arr->size-1] = 0;
    if(arr->size<=arr->len-16-1){
        arr->len -= 16; 
        arr->arr[arr->len] = 0;
        dyn_arr_char tmp;
        tmp.arr = malloc(sizeof(char)*(arr->len));
        memcpy(tmp.arr,arr->arr,sizeof(char)*arr->size);
        free(arr->arr);
        arr->arr = tmp.arr;
    }
}


void insert_end(dyn_arr_char *arr,char *value,int str_size){
    if(arr->size==arr->len-1){
        arr->len += 16;
        dyn_arr_char tmp;
        tmp.arr = malloc(sizeof(char)*(arr->len));
        memcpy(tmp.arr,arr->arr,sizeof(char)*arr->size);
        free(arr->arr);
        arr->arr = tmp.arr;
    }
    memcpy(arr->arr+arr->size-1,value,str_size+1);
    arr->size+=str_size;
}

char query(dyn_arr_char arr,int select){
    if(select>=arr.size){
        return 0;
    }
    char *p = arr.arr;
    return p[select];
}

int main(){
    dyn_arr_char my_arr;
    my_arr = init("abcd",4);
    my_arr.insert_end(&my_arr,"e",1);
    // my_arr.insert_end(&my_arr,7);
    // my_arr.insert_end(&my_arr,8);
    // my_arr.insert_end(&my_arr,9);
    // my_arr.insert_end(&my_arr,10);
    // for (int i = 11; i<=20 ; i++ ){
    //     my_arr.insert_end(&my_arr,i);
    // }

    my_arr.delet_end(&my_arr);
    
    printf("%s",my_arr.arr);

    // for(;;);
}
