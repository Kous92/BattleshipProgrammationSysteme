#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"

#define DEBUG

int main(int argc, char *argv[])
{   
    /* Make sure a port was specified. */
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    
    int lis_sockfd = setup_listener(atoi(argv[1])); /* Listener socket. */
    pthread_mutex_init(&mutexcount, NULL);

    while (1) 
    {
        if (player_count <= 252) 
        { /* Only launch a new game if we have room. Otherwise, just spin. */  
            int *cli_sockfd = (int *)malloc(2*sizeof(int)); /* Client sockets */
            memset(cli_sockfd, 0, 2*sizeof(int));
            
            /* Get two clients connected. */
            get_clients(lis_sockfd, cli_sockfd);
            
            #ifdef DEBUG
            printf("[DEBUG] Starting new game thread...\n");
            #endif

            pthread_t thread; /* Don't really need the thread id for anything in this case, but here it is anyway. */
            int result = pthread_create(&thread, NULL, run_game, (void *)cli_sockfd); /* Start a new thread for this game. */
            if (result){
                printf("Thread creation failed with return code %d\n", result);
                exit(-1);
            }
            
            #ifdef DEBUG
            printf("[DEBUG] New game thread started.\n");
            #endif
        }
    }

    close(lis_sockfd);

    pthread_mutex_destroy(&mutexcount);
    pthread_exit(NULL); 
}