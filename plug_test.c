// #define __GNUC__

#include<stdio.h>
#include<x86_64-linux-gnu/sys/cdefs.h>

static int i = 0;

void __attribute__((constructor)) plugin_init(){
    printf("load%d\n",i);
    i++;
}

int __attribute__((destructor)) plugin_fini(){
    printf("unload\n");
}