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

int nombre_joueurs = 0;
pthread_mutex_t mutex_nombre_joueurs;

Joueur initialiserJoueur(int joueur)
{
    Joueur j;
    int i, k;

    j.grille = initialiserGrille();
    j.grille_attaque = initialiserGrille();

    // Allocation dynamique du tableau de bateaux
    j.bateaux = (Bateau *) malloc(5 *sizeof(Bateau));

    // Définition des 5 bateaux

    // Porte-avions (Carrier)
    j.bateaux[0].longueur_bateau = 5;
    j.bateaux[0].symbole = 1;
    j.bateaux[0].touche = FALSE;
    j.bateaux[0].coordonnees = (Coordonnees *) malloc(5 *sizeof(Coordonnees));
    j.bateaux[0].degats = (boolean *) malloc(5 *sizeof(boolean));
    j.bateaux[0].nom = "Porte-avions";

    // Croiseur (Battleship)
    j.bateaux[1].longueur_bateau = 4;
    j.bateaux[1].symbole = 2;
    j.bateaux[1].touche = FALSE;
    j.bateaux[1].coordonnees = (Coordonnees *) malloc(4 *sizeof(Coordonnees));
    j.bateaux[1].degats = (boolean *) malloc(4 *sizeof(boolean));
    j.bateaux[1].nom = "Croiseur";

    // Sous-marin (submarine)
    j.bateaux[2].longueur_bateau = 3;
    j.bateaux[2].symbole = 3;
    j.bateaux[2].touche = FALSE;
    j.bateaux[2].coordonnees = (Coordonnees *) malloc(3 *sizeof(Coordonnees));
    j.bateaux[2].degats = (boolean *) malloc(3 *sizeof(boolean));
    j.bateaux[2].nom = "Sous-marin 1";

    j.bateaux[3].longueur_bateau = 3;
    j.bateaux[3].symbole = 4;
    j.bateaux[3].touche = FALSE;
    j.bateaux[3].coordonnees = (Coordonnees *) malloc(3 *sizeof(Coordonnees));
    j.bateaux[3].degats = (boolean *) malloc(3 *sizeof(boolean));
    j.bateaux[3].nom = "Sous-marin 2";

    // Torpilleur (destroyer)
    j.bateaux[4].longueur_bateau = 2;
    j.bateaux[4].symbole = 5;
    j.bateaux[4].touche = FALSE;
    j.bateaux[4].coordonnees = (Coordonnees *) malloc(2 *sizeof(Coordonnees));
    j.bateaux[4].degats = (boolean *) malloc(2 *sizeof(boolean));
    j.bateaux[4].nom = "Torpilleur";

    j.nombre_bateaux = 5;
    j.joueur = joueur;

    for (i = 0; i < 5; i++)
    {
        if ((j.grille == NULL) || (j.bateaux == NULL) || (j.bateaux[i].coordonnees == NULL) || (j.bateaux[i].degats == NULL))
        {
            exit(0);
        }
    }

    j.porte_avions = 0;
    j.croiseurs = 0;
    j.sous_marins1 = 0;
    j.sous_marins2 = 0;
    j.torpilleurs = 0;
    j.cibles_touchees = 0;

    for (i = 0; i < j.nombre_bateaux; i++)
    {
        switch (j.bateaux[i].symbole)
        {
            case 1:
            j.porte_avions++;
            break;

            case 2:
            j.croiseurs++;
            break;

            case 3:
            j.sous_marins1++;
            break;

            case 4:
            j.sous_marins2++;
            break;

            case 5:
            j.torpilleurs++;
            break;

            default:
            break;
        }
    }

    for (i = 0; i < 5; i++)
    {
        printf(" -> Dégâts (bateau longueur %d): ", j.bateaux[i].longueur_bateau);

        for (k = 0; k < j.bateaux[i].longueur_bateau; k++)
        {   
            j.bateaux[i].degats[k] = FALSE;
            printf("%d ", j.bateaux[i].degats[k]);
        }

        printf("\n");
    }   

    return j;
}

void afficherStatistiquesJoueur(Joueur j1, Joueur j2)
{
    int bateaux = 0;
    int bateaux_ennemis = 0;

    printf(">>> Joueur: %d\n", j1.joueur);
    printf(" * Porte-avions: %d\n", j1.porte_avions);
    printf(" * Croiseurs: %d\n", j1.croiseurs);
    printf(" * Sous-marins: %d\n", (j1.sous_marins1 + j1.sous_marins2));
    printf(" * Torpilleurs: %d\n", j1.torpilleurs);

    bateaux = j1.porte_avions + j1.croiseurs + j1.sous_marins1 + j1.sous_marins2 + j1.torpilleurs;
    bateaux_ennemis = j2.porte_avions + j2.croiseurs + j2.sous_marins1 + j2.sous_marins2 + j2.torpilleurs;

    if (bateaux_ennemis > 1)
    {
        printf(" > Bateaux ennemis restants: %d / 5\n\n", bateaux_ennemis);
    }
    else
    {
        printf("-> L'ennemi n'a plus qu'un seul bateau, détruisez-le pour gagner la bataille.\n");
    }

    if (bateaux > 1)
    {
        printf("-> Il vous reste %d bateaux\n", bateaux);
    }
    else
    {
        printf("-> ATTENTION: Il ne vous reste plus qu'un seul bateau\n");
    } 
}

int **initialiserGrille()
{
    // Allocation dynamique de la première dimension du tableau 2D: les lignes
    int **grille = (int **) malloc(10 *sizeof(int *));
    int i, j;

    if (grille == NULL)
    {
        exit(0);
    }

    // Suite de l'allocation dynamique: 2nde dimension du tableau: les colonnes
    for (i = 0; i < 10; i++)
    {
        // Allocation du sous-tableau dans la case du premier tableau dynamique
        grille[i] = (int *) malloc(10 *sizeof(int));

        if (grille[i] == NULL)
        {
            printf("Erreur: l'allocation dynamique a échoué\n");
            exit(0);
        }
    }

    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            grille[i][j] = 0;
        }
    }

    return grille;
}

void afficherGrille(int **grille)
{
    int i, j;
    char *lettres = "ABCDEFGHIJ";

    if (grille == NULL)
    {
        exit(0);
    }

    for (i = 0; i < 10; i++)
    {
        if (i == 0)
        {
            printf("   ");
        }
        
        printf(" %c ", lettres[i]);
    }

    printf("\n");

    for (i = 0; i < 10; i++)
    {
        if (i < 9)
        {
            printf("%d  ", (i + 1));
        }
        else
        {
            printf("10 ");
        }

        for (j = 0; j < 10; j++)
        {
            switch (grille[i][j])
            {
                // Eau
                case 0:
                printf(" ~ ");
                break;

                // Porte-avions (Carrier)
                case 1:
                printf(" C ");
                break;

                // Croiseur (Croiseur)
                case 2:
                printf(" B ");
                break;

                // Sous-marin (Submarine)
                case 3:
                case 4:
                printf(" S ");
                break;

                // Torpilleur (Destroyer)
                case 5:
                printf(" D ");
                break;

                // Porte-avions touché
                case 6:
                printf(" c ");
                break;

                // Croiseur touché
                case 7:
                printf(" b ");
                break;

                // Sous-marin touché
                case 8:
                case 9:
                printf(" s ");
                break;

                // Torpilleur touché
                case 10:
                printf(" d ");
                break;

                // Cible ratée
                case -1:
                printf(" X ");
                break;

                // Cible touchée
                case -2:
                printf(" * ");
                break;

                default:
                printf(" - ");
                break;
            }
        }

        printf("\n");
    }
}

void erreur(const char *message)
{
    perror(message);
    pthread_exit(NULL);
}

// ==============================================
// Fonctions lecture socket
// ==============================================

// Lecture d'un int depuis un socket client.
int receptionInt(int cli_sockfd)
{
    int message = 0;
    int n = read(cli_sockfd, &message, sizeof(int));
    
    if ((n < 0) || (n != sizeof(int)))
    { 
       return -1;
    }
    
    #ifdef DEBUG
    printf("[DEBUG] Int reçu: %d\n", message);
    #endif 
    
    return message;
}

// =============================================
// Fonctions écriture socket
// =============================================

// Écriture et envoi d'un message vers un socket client.
void envoiMessageClient(int cli_sockfd, char *message)
{
    int n = write(cli_sockfd, message, strlen(message));

    if (n < 0)
    {
        erreur("Erreur lors de l'écriture du message au socket client");
    }
}


// Écriture et envoi d'un int vers un socket client.
void envoiIntClient(int cli_sockfd, int message)
{
    int n = write(cli_sockfd, &message, sizeof(int));

    if (n < 0)
    {
        erreur("Erreur lors de l'écriture du message au socket client");
    }
}

/* Writes a message to both client sockets. */
void envoiMessage2Clients(int *cli_sockfd, char *message)
{
    envoiMessageClient(cli_sockfd[0], message);
    envoiMessageClient(cli_sockfd[1], message);
}

/* Writes an int to both client sockets. */
void envoiInt2Clients(int *cli_sockfd, int message)
{
    envoiIntClient(cli_sockfd[0], message);
    envoiIntClient(cli_sockfd[1], message);
}

void envoyerGrille(int **grille, int sockfd)
{
    int i, j, n;
    int k = 0;
    char *resultat;
    char buffer[100];

    #ifdef DEBUG
    printf("[DEBUG] Grille: ");
    #endif

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; i++)
        {
            switch (grille[i][j])
            {
                // Eau
                case 0:
                buffer[k] = '~';
                break;

                // Porte-avions (Carrier)
                case 1:
                buffer[k] = 'C';
                break;

                // Croiseur (Croiseur)
                case 2:
                buffer[k] = 'B';
                break;

                // Sous-marin (Submarine)
                case 3:
                buffer[k] = 'R';
                break;

                case 4:
                buffer[k] = 'S';
                break;

                // Torpilleur (Destroyer)
                case 5:
                buffer[k] = 'D';
                break;

                // Porte-avions touché
                case 6:
                buffer[k] = 'c';
                break;

                // Croiseur touché
                case 7:
                buffer[k] = 'b';
                break;

                // Sous-marin touché
                case 8:
                buffer[k] = 'r';
                break;

                case 9:
                buffer[k] = 's';
                break;

                // Torpilleur touché
                case 10:
                buffer[k] = 'd';
                break;

                // Cible ratée
                case -1:
                buffer[k] = 'X';
                break;

                // Cible touchée
                case -2:
                buffer[k] = '*';
                break;

                default:
                buffer[k] = '-';
                break;
            }

            buffer[k] = grille[i][j];

            #ifdef DEBUG
            printf("%d", buffer[k]);
            #endif

            k++;
        }
    }

    #ifdef DEBUG
    printf("\n");
    #endif

    resultat = buffer;

    n = write(sockfd, resultat, strlen(resultat));

    if (n < 0)
    {
        erreur("ERREUR: L'envoi du message au socket client a échoué");
    }
}

int **recevoirGrille(int sockfd)
{
    char *message;
    memset(message, 0, 101);
    int n = read(sockfd, message, 100);
    int i, j, k = 0;
    int **grille = (int **) malloc(10 *sizeof(int *));

    if (grille == NULL)
    {
        exit(0);
    }

    // Suite de l'allocation dynamique: 2nde dimension du tableau: les colonnes
    for (i = 0; i < 10; i++)
    {
        // Allocation du sous-tableau dans la case du premier tableau dynamique
        grille[i] = (int *) malloc(10 *sizeof(int));

        if (grille[i] == NULL)
        {
            printf("Erreur: l'allocation dynamique a échoué\n");
            exit(0);
        }
    }

    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            grille[i][j] = 0;
        }
    }

    if ((n < 0) || (n != 100))
    {
        erreur("ERREUR: Échec de la réception du message envoyé du serveur");
    }

    #ifdef DEBUG
    printf("[DEBUG] Message reçu: %s\n", message);
    printf("[DEBUG] Grille: ");
    #endif

    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; i++)
        {
            switch (message[k])
            {
                // Eau
                case '~':
                grille[i][j] = 0;
                break;

                // Porte-avions (Carrier)
                case 'C':
                grille[i][j] = 1;
                break;

                // Croiseur (Croiseur)
                case 'B':
                grille[i][j] = 2;
                break;

                // Sous-marin (Submarine)
                case 'R':
                grille[i][j] = 3;
                break;

                case 'S':
                grille[i][j] = 4;
                break;

                // Torpilleur (Destroyer)
                case 'D':
                grille[i][j] = 5;
                break;

                // Porte-avions touché
                case 'c':
                grille[i][j] = 6;
                break;

                // Croiseur touché
                case 'b':
                grille[i][j] = 7;
                break;

                // Sous-marin touché
                case 'r':
                grille[i][j] = 8;
                break;

                case 's':
                grille[i][j] = 9;
                break;

                // Torpilleur touché
                case 'd':
                grille[i][j] = 10;
                break;

                // Cible ratée
                case 'X':
                grille[i][j] = -1;
                break;

                // Cible touchée
                case '*':
                grille[i][j] = -2;
                break;

                default:
                grille[i][j] = -3;
                break;
            }

            #ifdef DEBUG
            printf("%d", grille[i][j]);
            #endif

            k++;
        }
    }

    #ifdef DEBUG
    printf("\n");
    #endif

    return grille;
}

// ==========================================
// Fonctions de connexion
// ==========================================

// Configuration du socket listener du serveur
int definirListenerServeur(int port)
{
    int sockfd;
    struct sockaddr_in serv_addr;

    // Obtenir un socket pour effectuer l'écoute dessus
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        erreur("Erreur lors de l'ouverture du socket listener.");
    }
    
    // Mise à 0 de la mémoire pour les informations du serveur
    memset(&serv_addr, 0, sizeof(serv_addr));
    
	// Configuration des informations du serveur
    serv_addr.sin_family = AF_INET;	
    serv_addr.sin_addr.s_addr = INADDR_ANY;	
    serv_addr.sin_port = htons(port);		

    // Lancement du serveur
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        erreur("Erreur lors du lancement du socket listener.");
    }

    #ifdef DEBUG
    printf("[DEBUG] Listener défini, serveur lancé.\n");    
    #endif 

    /* Return the socket number. */
    return sockfd;
}

// Définition des sockets clients et des connexions
void recevoirClients(int lis_sockfd, int *cli_sockfd)
{
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    
    #ifdef DEBUG
    printf("[DEBUG] Écoute des clients...\n");
    #endif 

    // Écoute pour 2 clients
    int nombre_connexions = 0;

    // Mise en place d'un session de jeu
    while (nombre_connexions < 2)
    {
        // Écoute pour les clients
	    listen(lis_sockfd, 253 - nombre_joueurs);
        
        // Mise à 0 de la mémoire pour les informations du serveur
        memset(&cli_addr, 0, sizeof(cli_addr));

        clilen = sizeof(cli_addr);
	
	    // Accepter la connexion du client.
        cli_sockfd[nombre_connexions] = accept(lis_sockfd, (struct sockaddr *) &cli_addr, &clilen);
        
        // Échec
        if (cli_sockfd[nombre_connexions] < 0)
        {
           erreur("Erreur lors de l'acceptation de la connexion d'un client."); 
        }   

        #ifdef DEBUG
        printf("[DEBUG] Connexion acceptée du client %d\n", nombre_connexions);
        #endif 
        
        // Envoyer au client son ID
        write(cli_sockfd[nombre_connexions], &nombre_connexions, sizeof(int));
        
        #ifdef DEBUG
        printf("[DEBUG] Sent client %d it's ID.\n", nombre_connexions); 
        #endif 
        
        // Pour incrémenter le nombre de joueurs de façon synchronisée, un mutex est nécessaire
        pthread_mutex_lock(&mutex_nombre_joueurs);
        nombre_joueurs++;
        printf("Joueurs: %d.\n", nombre_joueurs);
        pthread_mutex_unlock(&mutex_nombre_joueurs);

        if (nombre_connexions == 0) 
        {
            // Envoi de "HLD" au premier client pour informer l'utilisateur que le serveur attend un second client
            envoiMessageClient(cli_sockfd[0], "HLD");
            
            #ifdef DEBUG
            printf("[DEBUG] Le client 0 est informé de l'attente.\n");
            #endif 
        }

        nombre_connexions++;
    }
}

//================================
// Fonctions de jeu
// ==============================
Coordonnees attaqueJoueur(int cli_sockfd)
{
    #ifdef DEBUG
    printf("[DEBUG] Getting player attack (x position)...\n");
    #endif

    Coordonnees attaque;

    envoiMessageClient(cli_sockfd, "TRN");

    attaque.x = receptionInt(cli_sockfd);
    attaque.y = receptionInt(cli_sockfd);

    return attaque;
}

boolean verifierAttaque(int **grille_attaque, Coordonnees cible)
{
    boolean attaque = FALSE;
    int i, j;

    #ifdef DEBUG
    printf("[DEBUG] Missile (%d, %d) ", (cible.x - 1), (cible.y - 1));
    #endif

    switch (grille_attaque[cible.x - 1][cible.y - 1])
    {
        case 0:
        printf("> Attaque ratée !\n");
        attaque = FALSE;
        break;

        case 1:
        printf("> Porte-avions touché !\n");
        attaque = TRUE;
        break;

        case 2:
        printf("> Croiseur touché !\n");
        attaque = TRUE;
        break;

        case 3:
        printf("> Sous-marin touché !\n");
        attaque = TRUE;
        break;

        case 4:
        printf("> Sous-marin touché !\n");
        attaque = TRUE;
        break;

        case 5:
        printf("> Torpilleur touché !\n");
        attaque = TRUE;
        break;

        default:
        attaque = FALSE;
        break;
    }

    return attaque;
}

int **mettreAJourGrille(int **grille, Coordonnees cible, boolean attaque)
{
    switch (attaque)
    {
        // Cible touchée
        case TRUE:
        grille[cible.x - 1][cible.y - 1] = -2;

        #ifdef DEBUG
        printf("[DEBUG] Cible touchée en (%d, %d): \n", (cible.x - 1), (cible.y - 1));
        #endif 

        break;

        case FALSE:
        grille[cible.x - 1][cible.y - 1] = -1;

        #ifdef DEBUG
        printf("[DEBUG] Cible ratée en (%d, %d): \n", (cible.x - 1), (cible.y - 1));
        #endif

        break;

        default:
        break;
    }

    return grille;
}

int **mettreAJourGrilleBateauTouche(int **grille, Coordonnees cible, int symbole)
{
    switch (symbole)
    {
        // Cible touchée
        case 1:
        grille[cible.x - 1][cible.y - 1] = 6; // 'C' -> 'c'

        #ifdef DEBUG
        printf("[DEBUG] Porte-avions touché en (%d, %d), symbole 1 -> %d: \n", (cible.x - 1), (cible.y - 1), grille[cible.x - 1][cible.y - 1]);
        #endif

        break;

        case 2:
        grille[cible.x - 1][cible.y - 1] = 7; // 'B' -> 'b'

        #ifdef DEBUG
        printf("[DEBUG] Croiseur touché en (%d, %d), symbole 2 -> %d: \n", (cible.x - 1), (cible.y - 1), grille[cible.x - 1][cible.y - 1]);
        #endif

        break;

        case 3:
        grille[cible.x - 1][cible.y - 1] = 8; // 'S' -> 's'

        #ifdef DEBUG
        printf("[DEBUG] Sous-marin touché en (%d, %d), symbole 3 -> %d: \n", (cible.x - 1), (cible.y - 1), grille[cible.x - 1][cible.y - 1]);
        #endif

        break;

        case 4:
        grille[cible.x - 1][cible.y - 1] = 9; // 'S' -> 's'

        #ifdef DEBUG
        printf("[DEBUG] Sous-marin touché en (%d, %d), symbole 4 -> %d: \n", (cible.x - 1), (cible.y - 1), grille[cible.x - 1][cible.y - 1]);
        #endif

        break;

        case 5:
        grille[cible.x - 1][cible.y - 1] = 10; // 'D' -> 'd'

        #ifdef DEBUG
        printf("[DEBUG] Torpilleur touché en (%d, %d), symbole 5 -> %d: \n", (cible.x - 1), (cible.y - 1), grille[cible.x - 1][cible.y - 1]);
        #endif

        // printf("X (%d, %d)", (cible.x - 1), (cible.y - 1));
        break;

        default:
        break;
    }

    return grille;
}

void mettreAJourBateauxJoueur(Joueur *joueur, Coordonnees position, int *symbole)
{
    int i, j;
    int attaques_restantes = 0;
    boolean case_verifiee = FALSE;

    for (i = 0; i < joueur->nombre_bateaux; i++)
    {
        for (j = 0; j < joueur->bateaux[i].longueur_bateau; j++)
        {   
            // Bateau touché
            if ((joueur->bateaux[i].coordonnees[j].x == (position.x - 1)) && (joueur->bateaux[i].coordonnees[j].y == (position.y - 1)))
            {
                printf("%s ennemi touché", joueur->bateaux[i].nom);
                
                if (case_verifiee == FALSE)
                {
                    joueur->bateaux[i].degats[j] = TRUE;
                }
                
                attaques_restantes = joueur->bateaux[i].longueur_bateau - nombreAttaqueSurBateaux(joueur->bateaux[i].degats, joueur->bateaux[i].longueur_bateau);
                
                if (attaques_restantes == 0)
                {
                    printf(" -> détruit\n");
                } 
                else
                {
                    printf(" -> Encore %d fois pour détruire le %s\n", attaques_restantes, joueur->bateaux[i].nom);
                    printf("\n");
                }

                if (case_verifiee == FALSE)
                {
                    *symbole = joueur->grille[position.x - 1][position.y - 1];
                    case_verifiee = TRUE;

                    #ifdef DEBUG
                    printf("[DEBUG] Symbole reconnu: %d\n", joueur->grille[position.x - 1][position.y - 1]);
                    #endif


                }
            }
        }   
    }
}

// Dessiner les grilles dans stdout
void afficherGrillesJeu(int **grille, int **grille_attaque)
{
    int i, j;
    char *lettres = "ABCDEFGHIJ";

    if (grille == NULL)
    {
        exit(0);
    }

    if (grille_attaque == NULL)
    {
        exit(0);
    }

    printf("          Grille principale");
    printf("                ");
    printf("Grille d'attaque\n");

    // Grille des bateaux
    for (i = 0; i < 10; i++)
    {
        if (i == 0)
        {
            printf("   ");
        }
        
        printf(" %c ", lettres[i]);
    }

    // Grille d'attaque
    for (i = 0; i < 10; i++)
    {
        if (i == 0)
        {
            printf("   ");
        }
        
        printf(" %c ", lettres[i]);
    }

    printf("\n");

    // Lignes
    for (i = 0; i < 10; i++)
    {
        if (i < 9)
        {
            printf("%d  ", (i + 1));
        }
        else
        {
            printf("10 ");
        }

        // Colonnes: grille des bateaux
        for (j = 0; j < 10; j++)
        {
            switch (grille[i][j])
            {
                // Eau
                case 0:
                printf(" ~ ");
                break;

                // Porte-avions (Carrier)
                case 1:
                printf(" C ");
                break;

                // Croiseur (Croiseur)
                case 2:
                printf(" B ");
                break;

                // Sous-marin (Submarine)
                case 3:
                case 4:
                printf(" S ");
                break;

                // Torpilleur (Destroyer)
                case 5:
                printf(" D ");
                break;

                // Porte-avions touché
                case 6:
                printf(" c ");
                break;

                // Croiseur touché
                case 7:
                printf(" b ");
                break;

                // Sous-marin touché
                case 8:
                case 9:
                printf(" s ");
                break;

                // Torpilleur touché
                case 10:
                printf(" d ");
                break;

                // Cible ratée
                case -1:
                printf(" X ");
                break;

                // Cible touchée
                case -2:
                printf(" * ");
                break;

                default:
                printf(" - ");
                break;
            }
        }

        printf("   ");

        // Colonnes: grille d'attaque
        for (j = 0; j < 10; j++)
        {
            switch (grille_attaque[i][j])
            {
                // Eau
                case 0:
                printf(" ~ ");
                break;

                // Cible ratée
                case -1:
                printf(" X ");
                break;

                // Cible touchée
                case -2:
                printf(" * ");
                break;

                default:
                printf(" - ");
                break;
            }
        }

        printf("\n");
    }

    printf("\n");
}

/* Sends a board update to both clients. */
void envoyerMiseAJour(int *cli_sockfd, int attaqueX, int attaqueY, int id_joueur)
{
    #ifdef DEBUG
    printf("[DEBUG] Envoi de la mise à jour...\n");
    #endif
    
    // Signaler une mise à jour    
    envoiMessage2Clients(cli_sockfd, "UPD");

    /* Send the id of the player that made the move. */
    envoiInt2Clients(cli_sockfd, id_joueur);
    
    // Envoi de l'attaque
    envoiInt2Clients(cli_sockfd, attaqueX);
    envoiInt2Clients(cli_sockfd, attaqueY);
    
    #ifdef DEBUG
    printf("[DEBUG] Mise à jour envoyée.\n");
    #endif
}

int nombreAttaqueSurBateaux(boolean *degats, int longueur_bateau)
{
    int i;
    int n = 0;

    for (i = 0; i < longueur_bateau; i++)
    {
        if (degats[i] == TRUE)
        {
            #ifdef DEBUG1
            printf("[DEBUG] %d + 1\n", n);
            #endif

            n++;
        }
    }

    return n;
}

boolean verifierToucheCoule(Joueur *joueur, int symbole)
{
    int i, k;
    int attaques_restantes = 0;
    int total_degats = 0;
    int bateaux_detruits = 0;
    boolean ok = FALSE;

    for (i = 0; i < joueur->nombre_bateaux; i++)
    {
        total_degats = nombreAttaqueSurBateaux(joueur->bateaux[i].degats, joueur->bateaux[i].longueur_bateau);
        attaques_restantes = joueur->bateaux[i].longueur_bateau - total_degats;

        #ifdef DEBUG
        printf("[DEBUG] Vérification du bateau %d (%s)... Dégâts: %d / %d\n", (i + 1), joueur->bateaux[i].nom, nombreAttaqueSurBateaux(joueur->bateaux[i].degats, joueur->bateaux[i].longueur_bateau), joueur->bateaux[i].longueur_bateau);
        #endif

        boolean ok = FALSE;

        if ((total_degats == joueur->bateaux[i].longueur_bateau) && (joueur->bateaux[i].symbole == symbole))
        {
            if ((joueur->bateaux[i].symbole == 1) && (strcmp(joueur->bateaux[i].nom, "Porte-avions") == 0) && (joueur->porte_avions > 0))
            {
                joueur->porte_avions--;
                ok = TRUE;
                printf("Touché coulé pour le %s\n", joueur->bateaux[i].nom);
                // joueur->nombre_bateaux--;
            }   

            if ((joueur->bateaux[i].symbole == 2) && (strcmp(joueur->bateaux[i].nom, "Croiseur") == 0) && (joueur->croiseurs > 0))
            {
                joueur->croiseurs--;
                ok = TRUE;
                printf("Touché coulé pour le %s\n", joueur->bateaux[i].nom);
                // joueur->nombre_bateaux--;
            }   

            if ((joueur->bateaux[i].symbole == 3) && (strcmp(joueur->bateaux[i].nom, "Sous-marin 1") == 0) && (joueur->sous_marins1 > 0))
            {
                joueur->sous_marins1--;
                ok = TRUE;
                printf("Touché coulé pour le %s\n", joueur->bateaux[i].nom);
                // joueur->nombre_bateaux--;
            }   

            if ((joueur->bateaux[i].symbole == 4) && (strcmp(joueur->bateaux[i].nom, "Sous-marin 2") == 0) && (joueur->sous_marins2 > 0))
            {
                joueur->sous_marins2--;
                ok = TRUE;
                printf("Touché coulé pour le %s\n", joueur->bateaux[i].nom);
                // joueur->nombre_bateaux--;
            }

            if ((joueur->bateaux[i].symbole == 5) && (strcmp(joueur->bateaux[i].nom, "Torpilleur") == 0) && (joueur->torpilleurs > 0))
            {
                joueur->torpilleurs--;
                ok = TRUE;
                printf("Touché coulé pour le %s\n", joueur->bateaux[i].nom);
                // joueur->nombre_bateaux--;
            }
        }
        else
        {
            #ifdef DEBUG
            printf(" -> Encore %d fois pour détruire le %s\n", attaques_restantes, joueur->bateaux[i].nom);

            printf("[DEBUG] Dégâts (bateau longueur %d): ", joueur->bateaux[i].longueur_bateau);

            for (k = 0; k < joueur->bateaux[i].longueur_bateau; k++)
            {   
                printf("%d ", joueur->bateaux[i].degats[k]);
            }

            printf("\n");

            #endif
        }   
    }

    return ok;
}

boolean verifierGrilleAttaque(int **grille_attaque, Coordonnees attaque)
{
    boolean cible_non_faite = FALSE;

    switch (grille_attaque[attaque.x - 1][attaque.y - 1])
    {
        case 0:
        cible_non_faite = TRUE;
        break;

        case -1:
        case -2:
        cible_non_faite = FALSE;
        break;

        default:
        cible_non_faite = FALSE;
        break;
    }

    return cible_non_faite;
}

boolean victoire(Joueur j1, Joueur j2)
{
    int bateaux1 = 0;
    int bateaux2 = 0;

    bateaux1 = j1.porte_avions + j1.croiseurs + j1.sous_marins1 + j1.sous_marins2 + j1.torpilleurs;
    bateaux2 = j2.porte_avions + j2.croiseurs + j2.sous_marins1 + j2.sous_marins2 + j2.torpilleurs;

    #ifdef DEBUG
    printf("[DEBUG] Joueur 1 > bateaux: %d, attaques réussies: %d\n", bateaux1, j1.cibles_touchees);
    printf("[DEBUG] Joueur 2 > bateaux: %d, attaques réussies: %d\n", bateaux2, j2.cibles_touchees);
    #endif

    if ((bateaux1 == 0) || (j1.cibles_touchees == 17))
    {
        printf("Partie terminée: victoire du joueur %d\n", j2.joueur);

        return TRUE;
    }
    else if ((bateaux2 == 0) || (j2.cibles_touchees == 17))
    {
        printf("Partie terminée: victoire du joueur %d\n", j1.joueur);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Checks the board to determine if there is a winner. */
int check_board(char board[][3], int last_move)
{
    #ifdef DEBUG
    printf("[DEBUG] Checking for a winner...\n");
    #endif
   
    int row = last_move/3;
    int col = last_move%3;

    if ( board[row][0] == board[row][1] && board[row][1] == board[row][2] ) { /* Check the row for a win. */
        #ifdef DEBUG
        printf("[DEBUG] Win by row %d.\n", row);
        #endif 
        return 1;
    }
    else if ( board[0][col] == board[1][col] && board[1][col] == board[2][col] ) { /* Check the column for a win. */
        #ifdef DEBUG
        printf("[DEBUG] Win by column %d.\n", col);
        #endif 
        return 1;
    }
    else if (!(last_move % 2)) { /* If the last move was at an even numbered position we have to check the diagonal(s) as well. */
        if ( (last_move == 0 || last_move == 4 || last_move == 8) && (board[1][1] == board[0][0] && board[1][1] == board[2][2]) ) {  /* Check backslash diagonal. */
            #ifdef DEBUG
            printf("[DEBUG] Win by backslash diagonal.\n");
            #endif 
            return 1;
        }
        if ( (last_move == 2 || last_move == 4 || last_move == 6) && (board[1][1] == board[0][2] && board[1][1] == board[2][0]) ) { /* Check frontslash diagonal. */
            #ifdef DEBUG
            printf("[DEBUG] Win by frontslash diagonal.\n");
            #endif 
            return 1;
        }
    }

    #ifdef DEBUG
    printf("[DEBUG] No winner, yet.\n");
    #endif
    
    /* No winner, yet. */
    return 0;
}

/* Runs a game between two clients. */
void *run_game(void *thread_data) 
{
    int *cli_sockfd = (int *)thread_data; // Sockets clients. */
    
    Joueur j1 = initialiserJoueur(1);
    Joueur j2 = initialiserJoueur(2);
    Coordonnees attaque;

    printf("Début de la partie\n");
    
    // Envoi du message start
    envoiMessage2Clients(cli_sockfd, "SRT");

    #ifdef DEBUG
    printf("[DEBUG] Sent start message.\n");
    #endif

    j1.grille = recevoirGrille(cli_sockfd[0]);
    j1.grille_attaque = recevoirGrille(cli_sockfd[0]);
    j2.grille = recevoirGrille(cli_sockfd[1]);
    j2.grille_attaque = recevoirGrille(cli_sockfd[1]);

    envoyerGrille(j1.grille, cli_sockfd[1]);
    envoyerGrille(j2.grille, cli_sockfd[0]);
    envoyerGrille(j1.grille_attaque, cli_sockfd[1]);
    envoyerGrille(j2.grille_attaque, cli_sockfd[0]);

    afficherGrillesJeu(j1.grille, j1.grille_attaque);
    afficherGrillesJeu(j2.grille, j2.grille_attaque);
    
    int prev_player_turn = 1;
    int player_turn = 0;
    boolean game_over = FALSE;
    int turn_count = 0;
    boolean touche = FALSE, touche_coule = FALSE, cible_non_faite = FALSE, fin_partie = FALSE;
    int i = 0;
    int symbole = 0;

    while (game_over == FALSE) 
    {
        /* Tell other player to wait, if necessary. */
        if (prev_player_turn != player_turn)
        {
            envoiMessageClient(cli_sockfd[(player_turn + 1) % 2], "WAT");
        }

        int valide_x = 0;
        int valide_y;
        int move_x = 0;
        int move_y = 0;

        attaque = attaqueJoueur(cli_sockfd[player_turn]);

        if ((attaque.x == -1) || (attaque.y == -1)) break; /* Error reading client socket. */

        printf("Attaque du joueur %d à la position (%d, %d)\n", player_turn, attaque.x, attaque.y);

        switch (player_turn)
        {
            case 0:
            afficherGrillesJeu(j1.grille, j1.grille_attaque);
            break;

            case 1:
            afficherGrillesJeu(j2.grille, j2.grille_attaque);
            break;

            default:
            break;
        }

        touche = verifierAttaque(j1.grille_attaque, attaque);

	    if ((attaque.x == -1) || (attaque.y == -1))
        { /* Error reading from client. */
            printf("Joueur déconnecté.\n");
            break;
        }
        else 
        {
            // Mettre à jour la grille et envoyer la mise à jour
            switch (player_turn)
            {
                case 0:
                mettreAJourGrille(j1.grille_attaque, attaque, touche);

                if (touche == TRUE)
                {
                    j1.cibles_touchees++;
                    mettreAJourBateauxJoueur(&j2, attaque, &symbole);

                    #ifdef DEBUG
                    printf("[DEBUG] Symbole cible touchée: %d\n", symbole);
                    #endif

                    j2.grille = mettreAJourGrilleBateauTouche(j2.grille, attaque, symbole);

                    touche_coule = verifierToucheCoule(&j2, symbole);
                }

                break;

                case 1:
                mettreAJourGrille(j2.grille_attaque, attaque, touche);

                if (touche == TRUE)
                {
                    j2.cibles_touchees++;
                    mettreAJourBateauxJoueur(&j1, attaque, &symbole);

                    #ifdef DEBUG
                    printf("[DEBUG] Symbole cible touchée: %d\n", symbole);
                    #endif

                    j1.grille = mettreAJourGrilleBateauTouche(j1.grille, attaque, symbole);

                    touche_coule = verifierToucheCoule(&j1, symbole);
                }
                break;

                default:
                break;
            }

            envoyerMiseAJour(cli_sockfd, attaque.x, attaque.y, player_turn);
                
            switch (player_turn)
            {
                case 0:
                afficherGrillesJeu(j1.grille, j1.grille_attaque);
                break;

                case 1:
                afficherGrillesJeu(j2.grille, j2.grille_attaque);
                break;

                default:
                break;
            }

            /* Check for a winner/loser. */
            game_over = victoire(j1, j2);
            
            if (game_over == TRUE) 
            { /* We have a winner. */
                envoiMessageClient(cli_sockfd[player_turn], "WIN");
                envoiMessageClient(cli_sockfd[(player_turn + 1) % 2], "LSE");
                printf("Fin de partie: victoire du %d.\n", player_turn);
            }

            /* Move to next player. */
            prev_player_turn = player_turn;
            player_turn = (player_turn + 1) % 2;
            turn_count++;
        }
    }

    printf("Game over.\n");

	// Fermer les sockets clients et decrémenter le compteur de joueurs.
    close(cli_sockfd[0]);
    close(cli_sockfd[1]);

    // Synchronisation du nombre de joueurs par mutex: blocage du mutex
    pthread_mutex_lock(&mutex_nombre_joueurs);

    // Section critique
    nombre_joueurs--;
    printf("Number of players is now %d.", nombre_joueurs);
    nombre_joueurs--;
    printf("Number of players is now %d.", nombre_joueurs);

    // Fin de la décrémentation: déblocage du mutex
    pthread_mutex_unlock(&mutex_nombre_joueurs);
    
    free(cli_sockfd);

    pthread_exit(NULL);
}

/* 
 * Main Program
 */