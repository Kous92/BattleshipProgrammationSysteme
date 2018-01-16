#include "battleship.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define DEBUG

// Fonction de mise en pause
/*
void pause()
{
	char check;

	// Nettoyage du buffer
	while (check != '\n')
	{
		check = getchar();
	}

	printf("Appuyez sur une touche pour continuer\n");
	getchar();

	return;
}
*/

void viderBuffer()
{
    int c = 0;

    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

void erreur(const char *msg)
{
    #ifdef DEBUG
    perror(msg);
    #else
    printf("Soit le serveur a planté, soit le joueur s'est déconnecté.\nGAME OVER.\n");
    #endif 

    exit(0);
}

// Connexion au serveur
int connexionServeur(char *adresse_ip, int port)
{
	struct sockaddr_in adresse_serveur;
    struct hostent *serveur;
 
    // Obtention d'un socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    if (sockfd < 0) 
    {
        erreur("Erreur lors de l'ouverture du socket pour le serveur.");
    }
	
    // Obtenir l'adresse du serveur.
    serveur = gethostbyname(adresse_ip);
	
    if (serveur == NULL) 
    {
        fprintf(stderr,"ERREUR, aucun serveur disponible\n");
        exit(0);
    }
	
	// Mise à zéro de la mémoire pour les informations du serveur
	memset(&adresse_serveur, 0, sizeof(adresse_serveur));

	// Configuration des informations du serveur
    adresse_serveur.sin_family = AF_INET;
    memmove(serveur->h_addr, &adresse_serveur.sin_addr.s_addr, serveur->h_length);
    adresse_serveur.sin_port = htons(port); 

	/* Make the connection. */
    if (connect(sockfd, (struct sockaddr *) &adresse_serveur, sizeof(adresse_serveur)) < 0) 
    {
        erreur("Erreur lors de la connexion au serveur");
    }

    #ifdef DEBUG
    printf("[DEBUG] Connexion établie.\n");
    #endif 
    
    return sockfd;
}

// Lecture d'un int depuis un socket serveur.
int receptionInt(int sockfd)
{
    int message = 0;
    int n = read(sockfd, &message, sizeof(int));
    
    if ((n < 0) || (n != sizeof(int)))
    { 
        erreur("Erreur lors de la lecture du int depuis le socket serveur");
    }
    
    #ifdef DEBUG
    printf("[DEBUG] Int reçu: %d\n", message);
    #endif 
    
    return message;
}

void receptionMessage(int sockfd, char *message)
{
	/* Tous les messages sont sur 3 octets. */
    memset(message, 0, 4);
    int n = read(sockfd, message, 3);
    
    // Pas ce qu'on attendait, le serveur s'est éteint ou l'autre client s'est déconnecté
    if ((n < 0) || (n != 3))
    {
        erreur("Erreur lors de la lecture du int depuis le socket serveur.");
    }

    #ifdef DEBUG
    printf("[DEBUG] Message reçu: %s\n", message);
    #endif 
}

// Envoi d'un int vers le socket serveur
void envoyerInt(int sockfd, int message)
{
    int n = write(sockfd, &message, sizeof(int));

    if (n < 0)
    {
        erreur("Erreur lors de l'écriture de l'int vers le socket serveur");
    }
    
    #ifdef DEBUG
    printf("[DEBUG] Int écrit vers le serveur: %d\n", message);
    #endif 
}

// Envoi d'un message vers le socket serveur */
void envoiMessage(int cli_sockfd, char *message)
{
    int n = write(cli_sockfd, message, strlen(message));

    if (n < 0)
    {
        erreur("Erreur lors de l'envoi du message au socket client");
    }
}

void envoyerGrille(int **grille, int sockfd)
{
	int i, j, n;
	int k = 0;
	char *resultat;
	char buffer[100];

	#ifdef DEBUG
	printf("Envoi de la grille au serveur\n");
	printf("[DEBUG] Grille: ");
	#endif

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
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


			#ifdef DEBUG
			printf("%c", buffer[k]);
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
		erreur("ERREUR: L'envoi du message au socket serveur a échoué");
	}
}

int **recevoirGrille(int sockfd)
{
	#ifdef DEBUG
	printf("[DEBUG] Réception de la grille...\n");
	#endif

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
		#ifdef DEBUG
		printf("n = %d\n", n);
		#endif

		erreur("ERREUR: Échec de la réception du message envoyé du serveur");
	}

	#ifdef DEBUG
    printf("[DEBUG] Message reçu: %s\n", message);
	printf("[DEBUG] Grille: ");
    #endif

    for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
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
				grille[i][j] = 0;
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

void envoyerStatistiques(Joueur joueur, int sockfd)
{
	envoyerInt(sockfd, joueur.porte_avions);
	envoyerInt(sockfd, joueur.croiseurs);
	envoyerInt(sockfd, joueur.sous_marins1);
	envoyerInt(sockfd, joueur.sous_marins2);
	envoyerInt(sockfd, joueur.torpilleurs);
	envoyerInt(sockfd, joueur.nombre_bateaux);

}

void recevoirStatistiques(Joueur *joueur, int sockfd)
{
	joueur->porte_avions = receptionInt(sockfd);
	joueur->croiseurs = receptionInt(sockfd);
	joueur->sous_marins1 = receptionInt(sockfd);
	joueur->sous_marins2 = receptionInt(sockfd);
	joueur->torpilleurs = receptionInt(sockfd);
	joueur->nombre_bateaux = receptionInt(sockfd);

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

void placerBateauxAleatoirement(Joueur *joueur)
{
	int i = 0, j = 0, k = 0;
	int x, y;
	int direction = -1; // Vertical ou horizontal

	boolean ok = FALSE;

	Coordonnees position;
	Coordonnees *liste = (Coordonnees *) malloc(5 *sizeof(Coordonnees));

	printf("> Placement des bateaux en cours pour le joueur %d...\n", joueur->joueur);

	for (i = 0; i < 5; i++) 
	{
		ok = FALSE;

		while (ok == FALSE) 
		{
			direction = nombreAleatoire(0, 1); /* 0 -> horizontal, 1 -> vertical */
			position = genererPosition(direction, joueur->bateaux[i].longueur_bateau);

			if (validerPlacementBateaux(joueur->grille, position, direction, joueur->bateaux[i].longueur_bateau))
			{
				#ifdef DEBUG
			    printf("[DEBUG] Direction: %d\n", direction);
			    printf("     -> Bateau %d, longueur %d\n", (i + 1), joueur->bateaux[i].longueur_bateau);
			    printf("     -> Position = (%d, %d)\n", position.x, position.y);
			    printf("[DEBUG] Position validée");
			    #endif

				break;
			}
		}

		printf(" OK\n");

		if ((joueur->grille == NULL) || (joueur->bateaux == NULL))
		{
			exit(0);
		}


		joueur->grille = placerBateau(joueur->grille, joueur->bateaux[i], position, direction);

		// Enregistrement des coordonnées du bateau du joueur;
		#ifdef DEBUG		    
		printf("\n\n[DEBUG] Vérification des coordonnees du bateau %d (longueur %d)\n", (i + 1), joueur->bateaux[i].longueur_bateau);
		#endif

		j = 0;

		for (j = 0; j < joueur->bateaux[i].longueur_bateau; j++)
		{
			if (direction == 0)
			{
				joueur->bateaux[i].coordonnees[j].x = position.x;
				joueur->bateaux[i].coordonnees[j].y = position.y + j;

				#ifdef DEBUG		    
				printf("Horizontal: (%d, %d)\n", joueur->bateaux[i].coordonnees[j].x, joueur->bateaux[i].coordonnees[j].y);
				#endif
			}
			else
			{
				joueur->bateaux[i].coordonnees[j].x = (position.x + j);
				joueur->bateaux[i].coordonnees[j].y = position.y;

				#ifdef DEBUG		    
				printf("Vertical: (%d, %d)\n", joueur->bateaux[i].coordonnees[j].x, joueur->bateaux[i].coordonnees[j].y);
				#endif		
			}
			
			joueur->bateaux[i].degats[j] = FALSE;
		}

		printf("\n");

		#ifdef DEBUG

		printf("[DEBUG] Dégâts (bateau longueur %d): ", joueur->bateaux[i].longueur_bateau);

		for (j = 0; j < joueur->bateaux[i].longueur_bateau; j++)
		{
			printf("%d ", joueur->bateaux[i].degats[j]);
		}

		printf("\n");		    
		#endif	

		// pause();
		
	}

	free(liste);
	// return joueur->grille;
}

boolean validerPlacementBateaux(int **grille, Coordonnees position, int direction, int longueur_bateau)
{
	int i;
	boolean valide = TRUE;

	// printf("Validation...");

	for (i = 0; valide && i < longueur_bateau; i++) 
	{
		if (direction == 0) 
		{
			if ((grille[position.x][position.y + i] != 0) && ((position.y + i) < 10))
			{
				valide = FALSE;
			}
		} 
		else 
		{
			if ((grille[position.x + i][position.y] != 0) && ((position.y + i) < 10))
			{
				valide = FALSE;
			}
		}
	}

	return valide;
}

int **placerBateau(int **grille, Bateau bateau, Coordonnees position, int direction)
{
	int i;

	#ifdef DEBUG
	printf("[DEBUG] Placement... Bateau longueur %d\n", bateau.longueur_bateau);
	#endif

	for (i = 0; i < bateau.longueur_bateau; i++) 
	{
		if (direction == 0) 
		{	
			grille[position.x][position.y + i] = bateau.symbole;

			#ifdef DEBUG
			printf(" -> Horizontal: (%d, %d), symbole %d\n", position.x, (position.y + i), grille[position.x][position.y + i]);
			#endif
		}
		else 
		{
			grille[position.x + i][position.y] = bateau.symbole;

			#ifdef DEBUG
			printf(" -> Vertical: (%d, %d), symbole %d\n", (position.x + i), position.y, grille[position.x + i][position.y]);
			#endif
		}			
	}

	return grille;
}

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

int definirJoueurDepart()
{
	int tour = 0;
	tour = nombreAleatoire(1, 2);

	printf("> Le joueur %d commence la partie\n", tour);

	return tour;
}

// ==============================================================================
// JEU
// ==============================================================================
int nombreAleatoire(int min, int max)
{
	int n = 0;

	srand(time(NULL));
	n = (rand() % (max - min + 1)) + min;

	return n;
}

Coordonnees genererPosition(int direction, int longueur_bateau)
{
	Coordonnees position;

	// 0: horizontal, 1: vertical;
	if (direction == 0) 
	{
		position.x = nombreAleatoire(0, 9);
		position.y = nombreAleatoire(0, 9 - longueur_bateau);
		// printf("Horizontal: x = %d, y = %d\n", position.x, position.y);
	} 
	else 
	{
		position.x = nombreAleatoire(0, 9 - longueur_bateau);
		position.y = nombreAleatoire(0, 9);
		// printf("Vertical: x = %d, y = %d\n", position.x, position.y);
	}

	return position;
}

// ==============================================================================
// ATTAQUE
// ==============================================================================
// Définir la cible où toucher l'ennemi
Coordonnees definirCible()
{
	Coordonnees cible;
	boolean check = FALSE;
	cible.x = 0;
	cible.y = 0;

	int coordonnee = 0;
	int i;
	char *dictionnaire_colonne = "ABCDEFGHIJ";
	char colonne;

	// Saisie sécurisée
	do 
   	{
    	printf("> Colonne de la cible (y): ");
    	check = scanf("%d", &cible.y);

	    if (check == FALSE)
	   	{
	        printf("Erreur: votre saisie est invalide !\n");
	        fpurge(stdin);
	    }
        else if ((cible.y < 1) || (cible.y > 10))
        {
        	printf("Erreur: votre saisie est invalide !\n");
        }

   	} while (((cible.y < 1) || (cible.y > 10)) || (check == FALSE));

	// Saisie sécurisée
	do 
   	{
    	printf("> Ligne de la cible (x): ");
    	check = scanf("%d", &cible.x);

	    if (check == FALSE)
	   	{
	        printf("Erreur: votre saisie est invalide !\n");
	        fpurge(stdin);
	    }
        else if ((cible.x < 1) || (cible.x > 10))
        {
        	printf("Erreur: votre saisie est invalide !\n");
        }

   	} while (((cible.x < 1) || (cible.x > 10)) || (check == FALSE));

   	for (i = 0; i < strlen(dictionnaire_colonne); i++)
   	{
   		if (i == (cible.y - 1))
   		{
   			printf("%c\n", dictionnaire_colonne[i]);
   			colonne = dictionnaire_colonne[i];
   		}
   	}

	printf("> Attaque en %c%d\n", colonne, cible.x);

	return cible;
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

// ==============================================================================
// JEU
// ==============================================================================
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

// Jeu
void jeu()
{
	char *adresse_ip = "127.0.0.1";
	int port = 8888;
	
	int sockfd = connexionServeur(adresse_ip, port);
	int id = receptionInt(sockfd);

	#ifdef DEBUG
    printf("[DEBUG] Client ID: %d\n", id);
    #endif 

	Coordonnees attaque;
	boolean touche = FALSE, touche_coule = FALSE, cible_non_faite = FALSE, fin_partie = FALSE;
	int i = 0;
	int symbole = 0;
	int tour_joueur = 0;
	char *nom_bateau;
	char msg[4];

	Joueur j1 = initialiserJoueur(1);
	Joueur j2 = initialiserJoueur(2);
	placerBateauxAleatoirement(&j1);
	envoyerGrille(j1.grille, sockfd);
	envoyerGrille(j1.grille_attaque, sockfd);
	
	// Attente pour le démarrage du jeu 
    do 
    {
        receptionMessage(sockfd, msg);

        if (!strcmp(msg, "HLD"))
        {
            printf("En attente d'un adversaire...\n");
        }

    } while (strcmp(msg, "SRT"));

    j2.grille = recevoirGrille(sockfd);
	j2.grille_attaque = recevoirGrille(sockfd);

    // printf()
	tour_joueur = definirJoueurDepart();
	// afficherGrillesJeu(j1.grille, j1.grille_attaque);
	// afficherGrillesJeu(j2.grille, j2.grille_attaque);
	pause();
	
	while (1)
	{
		system("clear");
		receptionMessage(sockfd, msg);

		cible_non_faite = FALSE;

		if (fin_partie == FALSE)
		{   
			if (!strcmp(msg, "TRN")) 
			{ 
				/* Take a turn. */
		        printf("Votre tour...\n");

		        afficherGrillesJeu(j1.grille, j1.grille_attaque);

				if (touche_coule == TRUE)
				{
					printf("L'ennemi a détruit un de vos bateaux\n");
					touche_coule = FALSE;
				}

				if ((touche == TRUE) && (touche_coule == FALSE))
				{
					printf("L'ennemi a touché un de vos bateaux\n");
					touche = FALSE;
				}

				afficherStatistiquesJoueur(j1, j2);

				do
				{
					attaque = definirCible();
  
					if (verifierGrilleAttaque(j1.grille_attaque, attaque) == FALSE)
					{
						printf("ERREUR: Cible (%d, %d) déjà tentée, définissez une autre cible\n", attaque.x, attaque.y);
					}
					else
					{
						cible_non_faite = TRUE;
					}

				} while (cible_non_faite == FALSE);

				envoyerInt(sockfd, attaque.x); 
				envoyerInt(sockfd, attaque.y); 

				touche = verifierAttaque(j2.grille, attaque);
				j1.grille_attaque = mettreAJourGrille(j1.grille_attaque, attaque, touche);
				envoyerGrille(j1.grille_attaque, sockfd);
				
				if (touche == TRUE)
				{
					j1.cibles_touchees++;
					mettreAJourBateauxJoueur(&j2, attaque, &symbole);

					#ifdef DEBUG
					printf("[DEBUG] Symbole cible touchée: %d\n", symbole);
					#endif

					j2.grille = mettreAJourGrilleBateauTouche(j2.grille, attaque, symbole);

					touche_coule = verifierToucheCoule(&j2, symbole);
					fin_partie = victoire(j1, j2);
				}
       		}
	        else if (!strcmp(msg, "CNT")) 
	        { /* Server is sending the number of active players. Note that a "TRN" message will always follow a "CNT" message. */
	            int num_players = receptionInt(sockfd);
	            printf("There are currently %d active players.\n", num_players); 
	        }
	        else if (!strcmp(msg, "UPD")) 
	        {
	            /* Server is sending a game board update. */
	              
	        }
	        else if (!strcmp(msg, "WAT")) 
	        { /* Wait for other player to take a turn. */
	            printf("En attente de l'attaque de votre adversaire...\n");
	        }
	        else if (!strcmp(msg, "WIN")) // Gagnant
	        {
	            printf("Fin de partie, vous avez gagné !\n");
	            break;
	        }
	        else if (!strcmp(msg, "LSE")) // Perdant
	        { 
	            printf("Fin de partie, vous avez perdu.\n");
	            break;
	        }
	        else // Cas anormal
	        {
            	erreur("Erreur inconnue.");
        	}	

        	/*
			switch (tour_joueur)
			{
				case 1:
				afficherGrillesJeu(j1.grille, j1.grille_attaque);

				if (touche_coule)
				{
					printf("L'ennemi a détruit un de vos bateaux\n");
					touche_coule = FALSE;
				}

				if (touche)
				{
					printf("L'ennemi a touché un de vos bateaux\n");
					touche = FALSE;
				}

				afficherStatistiquesJoueur(j1, j2);
				
				do
				{
					attaque = definirCible();

					if (verifierGrilleAttaque(j1.grille_attaque, attaque) == FALSE)
					{
						printf("ERREUR: Cible (%d, %d) déjà tentée, définissez une autre cible\n", attaque.x, attaque.y);
					}
					else
					{
						cible_non_faite = TRUE;
					}

				} while (cible_non_faite == FALSE);

				i++;

				touche = verifierAttaque(j2.grille, attaque);
				j1.grille_attaque = mettreAJourGrille(j1.grille_attaque, attaque, touche);
				
				if (touche == TRUE)
				{
					j1.cibles_touchees++;
					mettreAJourBateauxJoueur(&j2, attaque, &symbole);

					#ifdef DEBUG
					printf("[DEBUG] Symbole cible touchée: %d\n", symbole);
					#endif

					j2.grille = mettreAJourGrilleBateauTouche(j2.grille, attaque, symbole);

					touche_coule = verifierToucheCoule(&j2, symbole);
					fin_partie = victoire(j1, j2);
				}

				tour_joueur = 2;
				break;

				case 2:				
				afficherGrillesJeu(j2.grille, j2.grille_attaque);

				if (touche_coule)
				{
					printf("L'ennemi a détruit un de vos bateaux\n");
					touche_coule = FALSE;
				}

				if (touche)
				{
					printf("L'ennemi a touché un de vos bateaux\n");
					touche = FALSE;
				}

				afficherStatistiquesJoueur(j2, j1);

				do
				{
					attaque = definirCible();

					if (verifierGrilleAttaque(j2.grille_attaque, attaque) == FALSE)
					{
						printf("ERREUR: Cible (%d, %d) déjà tentée, définissez une autre cible\n", attaque.x, attaque.y);
					}
					else
					{
						cible_non_faite = TRUE;
					}

				} while (cible_non_faite == FALSE);

				i++;

				touche = verifierAttaque(j1.grille, attaque);
				j2.grille_attaque = mettreAJourGrille(j2.grille_attaque, attaque, touche);
				
				if (touche == TRUE)
				{
					j2.cibles_touchees++;
					mettreAJourBateauxJoueur(&j1, attaque, &symbole);

					#ifdef DEBUG
					printf("[DEBUG] Symbole cible touchée: %d\n", symbole);
					#endif

					j1.grille = mettreAJourGrilleBateauTouche(j1.grille, attaque, symbole);

					touche_coule = verifierToucheCoule(&j1, symbole);
					fin_partie = victoire(j1, j2);
				}

				tour_joueur = 1;
				break;

				default:
				break;
			} */
		}
		
		pause();
	}

	printf("Nombre de coups: %d\n", i);
}

/*
char *saisirCible()
{
	boolean check = FALSE;

	char *positionEntree = NULL;
 	char *chaine = (char *) malloc(3 *sizeof(char));
 	char substr;

	do
	{
		printf("> Votre cible: ");

		if (fgets(chaine, 4, stdin) != NULL)
	    {
	        positionEntree = strchr(chaine, '\n');

	        if (positionEntree != NULL)
	        {
	            *positionEntree = '\0';

	            if ((strlen(chaine) < 2) || (strlen(chaine) > 3))
	            {
	            	check = FALSE;
	            	printf(">>> Erreur: saisie invalide\n");
	            }
	            else
	            {

	            }
	        }
	        else
	        {
	            viderBuffer();
	            check = FALSE;
	        }

	        check = TRUE;
	    }
	    else
	    {
	        viderBuffer();
	       	check = FALSE;
	    }

	} while (check == FALSE);

	return chaine;
}

boolean verifierChaine(char *chaine)
{
	boolean ok = TRUE;
	char *dictionnaire_ligne = "ABCDEFGHIJ";
	char *dictionnaire_colonne = "1234567890";
	int i, j;

	// Ligne
	for (i = 0; i < strlen(dictionnaire_ligne); i++)
	{
		if (chaine[0] != dictionnaire_ligne[i])
		{
			ok = FALSE;
		}
	}

	// Colonne (1er chiffre)
	for (j = 0; j < strlen(dictionnaire_colonne); j++)
	{
		if (chaine[1] != dictionnaire_colonne[i])
		{
			ok = FALSE;
		}
	}

	if (strlen(chaine) == 3)
	{
		// Colonne (2ème chiffre, seul 0 est autorisé)
		if (chaine[1] == '1')
		{
			if (chaine[2] == '0')
			{
				ok = TRUE;
			}
			else
			{
				ok = FALSE;
			}
		}
		else
		{
			ok = FALSE;
		}
	}

	return ok;
}
*/