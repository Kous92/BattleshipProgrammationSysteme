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
    int player_count = 0;
    pthread_mutex_t mutexcount;

    /* Make sure a port was specified. */
    if (argc < 2) {
        fprintf(stderr,"ERREUR: Aucun port renseigné\n");
        exit(1);
    }

    int lis_sockfd = definirListenerServeur(atoi(argv[1])); /* Listener socket. */
    pthread_mutex_init(&mutexcount, NULL);

    printf(">>> SERVEUR BATAILLE NAVALE LANCÉ\n");

    while (1) 
    {
        // Ici, le serveur peut gérer jusqu'à 126 salons
        if (player_count <= 252) 
        { 
            // Une nouvelle partie se lance si on a un salon de 2 joueurs. Sinon, laisser tourner  
            int *cli_sockfd = (int *) malloc(2 *sizeof(int)); // Sockets client
            memset(cli_sockfd, 0, 2 *sizeof(int));
            
            /* Get two clients connected. */
            recevoirClients(lis_sockfd, cli_sockfd);
            
            #ifdef DEBUG
            printf("[DEBUG] Démarrage d'un nouveau thread de jeu...\n");
            #endif

            pthread_t thread; /* Don't really need the thread id for anything in this case, but here it is anyway. */
            int result = pthread_create(&thread, NULL, run_game, (void *)cli_sockfd); /* Start a new thread for this game. */

            if (result)
            {
                printf("Échec de la création du thread avec le code d'erreur %d\n", result);
                exit(-1);
            }
            
            #ifdef DEBUG
            printf("[DEBUG] Thread lancé, nouvelle partie.\n");
            #endif
        }
    }

    close(lis_sockfd);

    pthread_mutex_destroy(&mutexcount);
    pthread_exit(NULL); 
}