#include"mcrcon.h"
// #include"net.h"
// #include"mcrcon_packet.h"

int main(){
    int client;
    // if((client = net_init("172.17.0.2","33001")) == -1){
    //     printf("socket init failed");
    // }
    rc_packet *packet;
    client = net_init("172.17.0.2","30010");
    if(!rcon_auth(client,"password")){
        printf("rcon login failed");
        return 1;
    }
    if((packet = rcon_command(client,""))){
        printf("%s",packet->data);
    }
    // printf("OK");
    return 0;
}