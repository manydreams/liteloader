#include"include/mcrcon.h"

#include<dlfcn.h>
#include<stdio.h>

// #include"net.h"
// #include"mcrcon_packet.h"

extern int test(void);

int main(){
    // int client;
    // // if((client = net_init("172.17.0.2","33001")) == -1){
    // //     printf("socket init failed");
    // // }
    // rc_packet *packet;
    // client = net_init("172.17.0.2","30010");
    // if(!rcon_auth(client,"password")){
    //     printf("rcon login failed\n");
    //     return 1;
    // }
    // if((packet = rcon_command(client,""))){
    //     printf("%s",packet->data);
    // }

    void *plugin = dlopen("./plug.so",RTLD_LAZY);
    if(plugin == NULL){
        printf("dlopen failed : %s\n",dlerror());
        return 1;
    }

    void *plugin_func = dlsym(plugin,"test");
    
    if(plugin_func == NULL){
        printf("dlsym failed : %s\n",dlerror());
        return 1;
    }

    test();

    // printf("OK");
    return 0;
}