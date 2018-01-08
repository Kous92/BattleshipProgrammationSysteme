#include "battleship.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// Fonction de mise en pause
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

void viderBuffer()
{
    int c = 0;

    while (c != '\n' && c != EOF)
    {
        c = getchar();
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
				printf(" S ");
				break;

				// Torpilleur (Destroyer)
				case 4:
				printf(" D ");
				break;

				// Porte-avions touché
				case 5:
				printf(" c ");
				break;

				// Croiseur touché
				case 6:
				printf(" b ");
				break;

				// Sous-marin touché
				case 7:
				printf(" s ");
				break;

				// Torpilleur touché
				case 8:
				printf(" d ");
				break;

				// Cible ratée
				case -1:
				printf(" X ");
				break;

				// Cible touchée
				case 10:
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
				printf(" S ");
				break;

				// Torpilleur (Destroyer)
				case 4:
				printf(" D ");
				break;

				// Porte-avions touché
				case 5:
				printf(" c ");
				break;

				// Croiseur touché
				case 6:
				printf(" b ");
				break;

				// Sous-marin touché
				case 7:
				printf(" s ");
				break;

				// Torpilleur touché
				case 8:
				printf(" d ");
				break;

				// Cible ratée
				case -1:
				printf(" X ");
				break;

				// Cible touchée
				case 10:
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
				printf(" S ");
				break;

				// Torpilleur (Destroyer)
				case 4:
				printf(" D ");
				break;

				// Porte-avions touché
				case 5:
				printf(" c ");
				break;

				// Croiseur touché
				case 6:
				printf(" b ");
				break;

				// Sous-marin touché
				case 7:
				printf(" s ");
				break;

				// Torpilleur touché
				case 8:
				printf(" d ");
				break;

				// Cible ratée
				case -1:
				printf(" X ");
				break;

				// Cible touchée
				case 10:
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

int **placerBateauxAleatoirement(Bateau *bateaux, int **grille)
{
	int i, j, x, y;
	int direction = -1; // Vertical ou horizontal

	boolean ok = FALSE;

	Coordonnees position;

	printf("> Placement des bateaux en cours...\n");

	for (i = 0; i < 5; i++) 
	{
		ok = FALSE;

		while (ok == FALSE) 
		{
			direction = nombreAleatoire(0, 1); /* 0 -> horizontal, 1 -> vertical */
			// printf(">>> Direction = %d\n", direction);

			position = genererPosition(direction, bateaux[i].longueur_bateau);

			if (validerPlacementBateaux(grille, position, direction, bateaux[i].longueur_bateau))
			{
				ok = TRUE;
			}
		}

		grille = placerBateau(grille, bateaux[i], position, direction);
	}

	return grille;
}

boolean validerPlacementBateaux(int **grille, Coordonnees position, int direction, int longueur_bateau)
{
	int i;
	boolean valide = TRUE;
	Symbole symbole;

	for (i = 0; valide && i <= longueur_bateau; i++) 
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

	for (i = 0; i < bateau.longueur_bateau; i++) 
	{
		if (direction == 0) 
		{
			grille[position.x][position.y + i] = bateau.symbole;
		}
		else 
		{
			grille[position.x + i][position.y] = bateau.symbole;
		}			
	}

	return grille;
}

Joueur initialiserJoueur(int joueur)
{
	Joueur j;

	j.grille = initialiserGrille();
	j.grille_attaque = initialiserGrille();

	// Allocation dynamique du tableau de bateaux
	j.bateaux = (Bateau *) malloc(5 *sizeof(Bateau));

	// Définition des 5 bateaux

	// Porte-avions (Carrier)
	j.bateaux[0].longueur_bateau = 5;
	j.bateaux[0].symbole = 1;
	j.bateaux[0].touche = FALSE;

	// Croiseur (Battleship)
	j.bateaux[1].longueur_bateau = 4;
	j.bateaux[1].symbole = 2;
	j.bateaux[1].touche = FALSE;

	// Sous-marin (submarine)
	j.bateaux[2].longueur_bateau = 3;
	j.bateaux[2].symbole = 3;
	j.bateaux[2].touche = FALSE;
	j.bateaux[3].longueur_bateau = 3;
	j.bateaux[3].symbole = 3;
	j.bateaux[3].touche = FALSE;

	// Torpilleur (destroyer)
	j.bateaux[4].longueur_bateau = 2;
	j.bateaux[4].symbole = 4;
	j.bateaux[4].touche = FALSE;

	j.nombre_bateaux = 5;
	j.joueur = joueur;

	return j;
}

void afficherStatistiquesJoueur(Joueur j)
{
	int porte_avions = 0;
	int croiseurs = 0;
	int sous_marins = 0;
	int torpilleurs = 0;
	int i;

	for (i = 0; i < j.nombre_bateaux; i++)
	{
		switch (j.bateaux[i].symbole)
		{
			case 1:
			porte_avions++;
			break;

			case 2:
			croiseurs++;
			break;

			case 3:
			sous_marins++;
			break;

			case 4:
			torpilleurs++;
			break;

			default:
			break;
		}
	}

	printf(">>> Joueur: %d\n", j.joueur);
	printf(" * Porte-avions: %d\n", porte_avions);
	printf(" * Croiseurs: %d\n", croiseurs);
	printf(" * Sous-marins: %d\n", sous_marins);
	printf(" * Torpilleurs: %d\n\n", torpilleurs);
}

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

   	for (i = 0; i < strlen(dictionnaire_colonne); i++)
   	{
   		if (i == (cible.x - 1))
   		{
   			printf("%c\n", dictionnaire_colonne[i]);
   			colonne = dictionnaire_colonne[i];
   		}
   	}

	printf("> Attaque en %c%d\n", colonne, cible.y);

	return cible;
}

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