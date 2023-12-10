#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<wait.h>

// #include "include/mcrcon.h"
#include"include/plugin_load.h"
#include"include/thread_pool.h"
#include"include/event.h"
// int a (char *command)
// {
//     int sock = net_init("169.0.0.2","30010");
//     rcon_auth(sock,"password");
//     rc_packet *tmp = rcon_command(sock,command);
//     close(sock);
//     printf("%s\n",tmp->data);
//     return 0;
// }

// int init(){
//     char tmp[256];
//     short i = 0;
//     while (1)
//     {
//         fgets(&tmp,sizeof(tmp),stdin);
//         if(tmp[0] == 0x71){
//             return 1;
//         }
//         for (i = 0; tmp[i] != 0x0a; i++);
        
//         tmp[i] = 0x00;

//         a(&tmp);
//     }
//     return 0;
    
// }

// void t(void *a){
//     int *tmp = (int*)a;
//     printf("hello thread pool, %d\n",*tmp);
// }

int main(int argc, char const *argv[])
{
    // thread_pool_t *pool = NULL;
    // thread_pool_create(&pool,5);

    // // int j = 0;
    // // int *k = &j;
    // for (int i = 0; i < 10; i++)
    // {
    //     thread_pool_add_work(pool,(void*)t,(void*)&i);
    //     usleep(0.0);
    // }
    // thread_pool_quit(pool);
    // thread_pool_delet(pool);
    plugin_t *plg = NULL;

    thread_pool_t *tpool = NULL;

    thread_pool_create(&tpool,1);

    if (plugin_list_load(&plg,"./plugins/")){
        printf("error");
    }

    plugin_func(plg,tpool,"test.so","plugin_init",NULL);

    // thread_pool_quit(tpool);

    // thread_pool_delet(tpool);

    // plugin_unload(plg,"test.so");

    event_broadcast_t *event_bd = NULL;
    event_register(&event_bd,"player_said",plg);
    event_register(&event_bd,"player_left",plg->next);
    return 0;
}
