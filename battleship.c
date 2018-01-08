#include "battleship.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
				case 0:
				printf(" - ");
				break;

				case 1:
				printf(" C ");
				break;

				case 2:
				printf(" B ");
				break;

				case 3:
				printf(" S ");
				break;

				case 4:
				printf(" D ");
				break;

				default:
				printf(" - ");
				break;
			}
		}

		printf("\n");
	}
}

int **placerBateauxAleatoirement(Bateau *bateaux, int **grille)
{
	int i, j, x, y;
	int direction = -1; // Vertical ou horizontal

	boolean ok = FALSE;

	Coordonnees position;

	for (i = 0; i < 5; i++) 
	{
		ok = FALSE;

		while (ok == FALSE) 
		{
			direction = nombreAleatoire(0, 1); /* 0 -> horizontal, 1 -> vertical */
			printf(">>> Direction = %d\n", direction);

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

int nombreAleatoire(int min, int max)
{
	srand(time(NULL));
	nombreMystere = (rand() % (MAX - MIN + 1)) + MIN;

	return n;
}

Coordonnees genererPosition(int direction, int longueur_bateau)
{
	Coordonnees position;

	// 0: horizontal, 1: vertical;
	if (direction == 0) 
	{
		position.x = nombreAleatoire(0, 10);
		position.y = nombreAleatoire(0, 10 - longueur_bateau);
		printf("Horizontal: x = %d, y = %d\n", position.x, position.y);
	} 
	else 
	{
		position.x = nombreAleatoire(0, 10 - longueur_bateau);
		position.y = nombreAleatoire(0, 10);
		printf("Vertical: x = %d, y = %d\n", position.x, position.y);
	}

	return position;
}