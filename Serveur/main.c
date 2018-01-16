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
    int nombre_joueurs = 0;
    pthread_mutex_t mutex_compteur;

    // On s'assure qu'un port est renseigné
    if (argc < 2) 
    {
        fprintf(stderr,"ERREUR: Aucun port renseigné\n");
        exit(1);
    }

    int lis_sockfd = definirListenerServeur(atoi(argv[1])); // Socket listener
    pthread_mutex_init(&mutex_compteur, NULL);

    printf(">>> SERVEUR BATAILLE NAVALE LANCÉ\n");

    while (1) 
    {
        // Ici, le serveur peut gérer jusqu'à 126 salons
        if (nombre_joueurs <= 252) 
        { 
            // Une nouvelle partie se lance si on a un salon de 2 joueurs. Sinon, laisser tourner  
            int *cli_sockfd = (int *) malloc(2 *sizeof(int)); // Sockets clients
            memset(cli_sockfd, 0, 2 *sizeof(int));
            
            // Recevoir la connexion de 2 clients
            recevoirClients(lis_sockfd, cli_sockfd);
            
            #ifdef DEBUG
            printf("[DEBUG] Démarrage d'un nouveau thread de jeu...\n");
            #endif

            pthread_t thread;

            // Lancer un nouveau thread, un salon de jeu entre 2 joueurs
            int resultat = pthread_create(&thread, NULL, lancerJeu, (void *)cli_sockfd);

            if (resultat)
            {
                printf("Échec de la création du thread avec le code d'erreur %d\n", resultat);
                exit(-1);
            }
            
            #ifdef DEBUG
            printf("[DEBUG] Thread lancé, nouvelle partie.\n");
            #endif
        }
    }

    close(lis_sockfd); // Fermeture du socket serveur

    pthread_mutex_destroy(&mutex_compteur);
    pthread_exit(NULL); 
}