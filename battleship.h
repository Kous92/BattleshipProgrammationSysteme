#ifndef BATTLESHIP_H
#define BATTLESHIP_H

typedef enum
{
	TRUE = 1;
	FALSE = 0;
} boolean;

/*=========================================================================
  Valeurs des case de la grille
  - 1: Cible ratée ==> 'X'
  - 0: Eau ==> '-'
  - 1: Porte-avions (5 cases) ==> 'C' (Carrier)
  - 2: Croiseur (4 cases) ==> 'B' (Battleship)
  - 3: Sous-marin (3 cases) ==> 'S' (Submarine)
  - 4: Torpilleur (2 cases) ==> 'D' (Destroyer)
  - 5: Porte-avions touché ==> 'c'
  - 6: Croiseur touché ==> 'b'
  - 7: Sous-marin touché ==> 's'
  - 8: Torpilleur touché ==> 'd'
  =========================================================================*/
typedef enum
{
	CIBLE_RATEE = -1;
	EAU = 0;
	PORTE_AVIONS = 1;
	CROISEUR = 2;
	SOUS-MARIN = 3;
	TORPILLEUR = 4;
	PORTE_AVIONS_HIT = 5;
	CROISEUR_HIT = 6;
	SOUS-MARIN_HIT = 7;
	TORPILLEUR_HIT = 8;
} Symbole;

typedef struct Coordonnees
{
	int x;
	int y;
} Coordonnees;


// Fonction de mise en pause
void pause();
int **initialiserGrille();
void afficherGrille(int **grille);
void lancerBateaux(int **ships);
boolean validerPlacementBateaux(int **grille, Coordonnees position, int direction, int longueur_bateau)
int nombreAleatoire(int min, int max);
Coordonnees genererPosition(int direction, int longueur_bateau);

#endif