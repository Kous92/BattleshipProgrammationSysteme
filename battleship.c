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
				case 9:
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
				case 9:
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
				case 9:
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
		grille[cible.x - 1][cible.y - 1] = 9;
		// printf("* (%d, %d)", (cible.x - 1), (cible.y - 1));
		break;

		case FALSE:
		grille[cible.x - 1][cible.y - 1] = -1;
		// printf("X (%d, %d)", (cible.x - 1), (cible.y - 1));
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
		grille[cible.x - 1][cible.y - 1] = 5; // 'C' -> 'c'
		break;

		case 2:
		grille[cible.x - 1][cible.y - 1] = 6; // 'B' -> 'b'
		break;

		case 3:
		grille[cible.x - 1][cible.y - 1] = 7; // 'S' -> 's'
		break;

		case 4:
		grille[cible.x - 1][cible.y - 1] = 8; // 'D' -> 'd'
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

			// printf(">>> Bateau %d, longueur %d\n", (i + 1), joueur->bateaux[i].longueur_bateau);

			position = genererPosition(direction, joueur->bateaux[i].longueur_bateau);
			// printf(">>> Position = (%d, %d)\n", position.x, position.y);

			if (validerPlacementBateaux(joueur->grille, position, direction, joueur->bateaux[i].longueur_bateau))
			{
				printf("Break: valide");
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
		printf("\n\nVérification des coordonnees du bateau %d (longueur %d)\n", (i + 1), joueur->bateaux[i].longueur_bateau);

		j = 0;

		for (j = 0; j < joueur->bateaux[i].longueur_bateau; j++)
		{
			if (direction == 0)
			{
				joueur->bateaux[i].coordonnees[j].x = position.x;
				joueur->bateaux[i].coordonnees[j].y = position.y + j;
				printf("Horizontal: (%d, %d)\n", joueur->bateaux[i].coordonnees[j].x, joueur->bateaux[i].coordonnees[j].y);
			}
			else
			{
				joueur->bateaux[i].coordonnees[j].x = (position.x + j);
				joueur->bateaux[i].coordonnees[j].y = position.y;
				printf("Vertical: (%d, %d)\n", joueur->bateaux[i].coordonnees[j].x, joueur->bateaux[i].coordonnees[j].y);
			}
			
			joueur->bateaux[i].degats[j] = FALSE;
		}

		printf("\n");
		/*
		printf(" -> Dégâts: ");

		for (j = 0; j < joueur->bateaux[j].longueur_bateau; j++)
		{
			printf("%d ", joueur->bateaux[i].degats[j]);
		}

		printf("\n");
		*/
		// pause();
		
	}

	free(liste);
	// return joueur->grille;
}

boolean validerPlacementBateaux(int **grille, Coordonnees position, int direction, int longueur_bateau)
{
	int i;
	boolean valide = TRUE;
	Symbole symbole;

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

	if (valide == FALSE)
	{
		// printf("ERREUR\n");
	}
	else
	{
		// printf("OK\n");
	}

	return valide;
}

int **placerBateau(int **grille, Bateau bateau, Coordonnees position, int direction)
{
	int i;

	printf("Placement... Bateau longueur %d\n", bateau.longueur_bateau);

	for (i = 0; i < bateau.longueur_bateau; i++) 
	{
		if (direction == 0) 
		{
			printf("Horizontal: (%d, %d)\n", position.x, (position.y + i));
			grille[position.x][position.y + i] = bateau.symbole;
		}
		else 
		{
			printf("Vertical: (%d, %d)\n", (position.x + i), position.y);
			grille[position.x + i][position.y] = bateau.symbole;
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
	j.bateaux[2].nom = "Sous-marin";

	j.bateaux[3].longueur_bateau = 3;
	j.bateaux[3].symbole = 3;
	j.bateaux[3].touche = FALSE;
	j.bateaux[3].coordonnees = (Coordonnees *) malloc(3 *sizeof(Coordonnees));
	j.bateaux[3].degats = (boolean *) malloc(3 *sizeof(boolean));
	j.bateaux[3].nom = "Sous-marin";

	// Torpilleur (destroyer)
	j.bateaux[4].longueur_bateau = 2;
	j.bateaux[4].symbole = 4;
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
	j.sous_marins = 0;
	j.torpilleurs = 0;

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
			j.sous_marins++;
			break;

			case 4:
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
	printf(">>> Joueur: %d\n", j1.joueur);
	printf(" * Porte-avions: %d\n", j1.porte_avions);
	printf(" * Croiseurs: %d\n", j1.croiseurs);
	printf(" * Sous-marins: %d\n", j1.sous_marins);
	printf(" * Torpilleurs: %d\n", j1.torpilleurs);
	printf(" > Bateaux ennemis restants: %d\n\n", j2.nombre_bateaux);
}

void mettreAJourBateauxJoueur(Joueur *joueur, Coordonnees position, int *symbole)
{
	int i, j;
	int attaques_restantes = 0;

	for (i = 0; i < joueur->nombre_bateaux; i++)
	{
		for (j = 0; j < joueur->bateaux[i].longueur_bateau; j++)
		{	
			// Bateau touché
			if ((joueur->bateaux[i].coordonnees[j].x == (position.x - 1)) && (joueur->bateaux[i].coordonnees[j].y == (position.y - 1)))
			{
				printf("%s ennemi touché", joueur->bateaux[i].nom);
				joueur->bateaux[i].degats[j] = TRUE;

				attaques_restantes = joueur->bateaux[i].longueur_bateau - nombreAttaqueSurBateaux(joueur->bateaux[i].degats);
				
				if (attaques_restantes == 0)
				{
					printf(" -> détruit\n");
				} 
				else
				{
					// printf("Encore %d fois pour détruire le %s\n", joueur->bateaux[i].longueur_bateau - nombreAttaqueSurBateaux(joueur->bateaux[i].degats), joueur->bateaux[i].nom);
					printf("\n");
				}

				*symbole = joueur->grille[position.x - 1][position.y - 1];
			}
		}	
	}
}

void mettreAJourStatistiquesJoueur(Joueur *j)
{

}

int nombreAttaqueSurBateaux(boolean *degats)
{
	int i;
	int n = 0;

	for (i = 0; i < 5; i++)
	{
		if (degats[i] == TRUE)
		{
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

	printf("Missile (%d, %d) ", (cible.x - 1), (cible.y - 1));

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
		printf("> Torpilleur touché !\n");
		attaque = TRUE;
		break;

		default:
		attaque = FALSE;
		break;
	}

	return attaque;
}

boolean verifierToucheCoule(Joueur *joueur)
{
	int i, j;
	int attaques_restantes = 0;
	int total_degats = 0;
	int bateaux_detruits = 0;
	boolean ok = FALSE;

	for (i = 0; i < joueur->nombre_bateaux; i++)
	{
		printf(">>> Vérification du bateau %d (%s)... Dégâts: %d / %d\n", (i + 1), joueur->bateaux[i].nom, nombreAttaqueSurBateaux(joueur->bateaux[i].degats), joueur->bateaux[i].longueur_bateau);

		total_degats = joueur->bateaux[i].longueur_bateau - nombreAttaqueSurBateaux(joueur->bateaux[i].degats);

		if (total_degats == 0)
		{
			printf("Touché coulé pour le %s\n", joueur->bateaux[i].nom);
			ok = TRUE;

			if ((strcmp(joueur->bateaux[i].nom, "Porte-avions") == 0) && (joueur->porte_avions > 0))
			{
				joueur->porte_avions--;
				bateaux_detruits++;
			}	

			if ((strcmp(joueur->bateaux[i].nom, "Croiseur") == 0) && (joueur->croiseurs > 0))
			{
				joueur->croiseurs--;
				bateaux_detruits++;
			}	

			if ((joueur->bateaux[i].symbole == 3) && (joueur->sous_marins > 0))
			{
				joueur->sous_marins--;
				bateaux_detruits++;
			}	

			if ((strcmp(joueur->bateaux[i].nom, "Torpilleur") == 0) && (joueur->torpilleurs > 0))
			{
				joueur->torpilleurs--;
				bateaux_detruits++;
			}
		}
		else
		{
			// printf("Encore %d fois pour détruire le %s\n", total_degats, joueur->bateaux[i].nom);
		}	
	}

	joueur->nombre_bateaux = joueur->nombre_bateaux - bateaux_detruits++;

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
		case 9:
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
	if (j1.nombre_bateaux == 0)
	{
		printf("Partie terminée: victoire du joueur %d\n", j2.joueur);

		return TRUE;
	}
	else if (j2.nombre_bateaux == 0)
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
	Joueur j1 = initialiserJoueur(1);
	Joueur j2 = initialiserJoueur(2);
	Coordonnees attaque;
	boolean touche = FALSE, touche_coule = FALSE, cible_non_faite = FALSE, fin_partie = FALSE;
	int i = 0;
	int symbole = 0;
	int tour_joueur = 0;
	char *nom_bateaux;

	placerBateauxAleatoirement(&j1);
	placerBateauxAleatoirement(&j2);
	
	tour_joueur = definirJoueurDepart();
	afficherGrillesJeu(j1.grille, j1.grille_attaque);
	afficherGrillesJeu(j2.grille, j2.grille_attaque);
	pause();
	
	do
	{
		system("clear");
		cible_non_faite = FALSE;

		if (fin_partie == FALSE)
		{
			switch (tour_joueur)
			{
				case 1:
				afficherGrillesJeu(j1.grille, j1.grille_attaque);
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
					mettreAJourBateauxJoueur(&j2, attaque, &symbole);
					printf(" * Symbole: %d\n", symbole);
					j2.grille = mettreAJourGrilleBateauTouche(j2.grille, attaque, symbole);

					touche_coule = verifierToucheCoule(&j2);
					fin_partie = victoire(j1, j2);
				}

				tour_joueur = 2;
				break;

				case 2:				
				afficherGrillesJeu(j2.grille, j2.grille_attaque);
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
					mettreAJourBateauxJoueur(&j1, attaque, &symbole);
					printf(" * Symbole: %d\n", symbole);
					j1.grille = mettreAJourGrilleBateauTouche(j1.grille, attaque, symbole);

					touche_coule = verifierToucheCoule(&j1);
					fin_partie = victoire(j1, j2);
				}

				tour_joueur = 1;
				break;

				default:
				break;
			}
		}
		
		pause();

	} while (fin_partie == FALSE);
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