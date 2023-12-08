#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<wait.h>

#include "include/mcrcon.h"


int a(char *command)
{
    int sock = net_init("169.0.0.2","30010");
    rcon_auth(sock,"password");
    rc_packet *tmp = rcon_command(sock,command);
    close(sock);
    printf("%s\n",tmp->data);
    return 0;
}

int init(){
    char tmp[256];
    short i = 0;
    while (1)
    {
        fgets(&tmp,sizeof(tmp),stdin);
        if(tmp[0] == 0x71){
            return 1;
        }
        for (i = 0; tmp[i] != 0x0a; i++);
        
        tmp[i] = 0x00;

        a(&tmp);
    }
    return 0;
    
}

