#ifndef BATTLESHIP_H
#define BATTLESHIP_H

typedef enum
{
	TRUE = 1,
	FALSE = 0
} boolean;

/*=========================================================================
  Valeurs des case de la grille
  - -1: Cible ratée ==> 'X'
  - 0: Eau ==> '-'
  - 1: Porte-avions (5 cases) ==> 'C' (Carrier)
  - 2: Croiseur (4 cases) ==> 'B' (Battleship)
  - 3: Sous-marin (3 cases) ==> 'S' (Submarine)
  - 4: Torpilleur (2 cases) ==> 'D' (Destroyer)
  - 5: Porte-avions touché ==> 'c'
  - 6: Croiseur touché ==> 'b'
  - 7: Sous-marin touché ==> 's'
  - 8: Torpilleur touché ==> 'd'
  - 9: Cible touchée ==> '*'
  =========================================================================*/
typedef enum
{
	CIBLE_RATEE = -1,
	EAU = 0,
	PORTE_AVIONS = 1,
	CROISEUR = 2,
	SOUS_MARIN = 3,
	TORPILLEUR = 4,
	PORTE_AVIONS_HIT = 5,
	CROISEUR_HIT = 6,
	SOUS_MARIN_HIT = 7,
	TORPILLEUR_HIT = 8
} Symbole;

typedef struct Coordonnees
{
	int x;
	int y;
} Coordonnees;

typedef struct Bateau
{
	int longueur_bateau;
	int symbole;
	boolean touche;
} Bateau;

typedef struct Joueur
{
	Bateau *bateaux;
	int nombre_bateaux;
	int **grille;
	int **grille_attaque;
	int joueur;
} Joueur;

// Fonction de mise en pause
void pause();
void viderBuffer();

// Gestion de la grille
int **initialiserGrille();
void afficherGrille(int **grille);
void afficherGrillesJeu(int **grille, int **grille_attaque);

// Gestion des bateaux
int **placerBateauxAleatoirement(Bateau *bateaux, int **grille);
int **placerBateau(int **grille, Bateau bateau, Coordonnees position, int direction);
boolean validerPlacementBateaux(int **grille, Coordonnees position, int direction, int longueur_bateau);

// Joueurs
Joueur initialiserJoueur(int joueur);
void afficherStatistiquesJoueur(Joueur j);

// Coordonnées,...
int nombreAleatoire(int min, int max);
Coordonnees genererPosition(int direction, int longueur_bateau);
Coordonnees definirCible();
char *saisirCible();
boolean verifierChaine(char *chaine);

#endif