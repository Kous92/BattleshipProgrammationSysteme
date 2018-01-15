#ifndef BATTLESHIP_H
#define BATTLESHIP_H

typedef enum
{
	TRUE = 1,
	FALSE = 0
} boolean;

/*=========================================================================
  Valeurs des cases de la grille
  - -1: Cible ratée ==> 'X'
  - 0: Eau ==> ''
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
	Coordonnees *coordonnees;
	boolean *degats;
	int vies;
	char *nom;
} Bateau;

typedef struct Joueur
{
	Bateau *bateaux;
	int nombre_bateaux;
	int porte_avions;
	int croiseurs;
	int sous_marins1;
	int sous_marins2;
	int torpilleurs;
	int **grille;
	int **grille_attaque;
	int joueur;
	int cibles_touchees;
} Joueur;

typedef struct Grille
{
	int symbole;
	Coordonnees position;
} Grille;

// Fonction de mise en pause
// void pause();
void viderBuffer();
void erreur(const char *msg);

// Connexion au serveur
int connexionServeur(char *adresse_ip, int port);
int receptionInt(int sockfd);
void receptionMessage(int sockfd, char *message);
void envoyerInt(int sockfd, int message);
void envoiMessage(int cli_sockfd, char *message);
void envoyerGrille(int **grille, int sockfd);
int **recevoirGrille(int sockfd);
void envoyerStatistiques(Joueur joueur, int sockfd);
void recevoirStatistiques(Joueur *joueur, int sockfd);

// Gestion de la grille
int **initialiserGrille();
void afficherGrille(int **grille);
void afficherGrillesJeu(int **grille, int **grille_attaque);
int **mettreAJourGrille(int **grille, Coordonnees cible, boolean attaque);
int **mettreAJourGrilleBateauTouche(int **grille, Coordonnees cible, int symbole);

// Gestion des bateaux
void placerBateauxAleatoirement(Joueur *joueur);
int **placerBateau(int **grille, Bateau bateau, Coordonnees position, int direction);
boolean validerPlacementBateaux(int **grille, Coordonnees position, int direction, int longueur_bateau);

// Joueurs
Joueur initialiserJoueur(int joueur);
void afficherStatistiquesJoueur(Joueur j1, Joueur j2);
void mettreAJourBateauxJoueur(Joueur *j, Coordonnees position, int *symbole);
void mettreAJourStatistiquesJoueur(Joueur *j);
int nombreAttaqueSurBateaux(boolean *degats, int longueur_bateau);
int definirJoueurDepart();

// Coordonnées,...
int nombreAleatoire(int min, int max);
Coordonnees genererPosition(int direction, int longueur_bateau);

// Attaque
Coordonnees definirCible();
boolean verifierAttaque(int **grille_attaque, Coordonnees cible);
boolean verifierToucheCoule(Joueur *joueur, int symbole);
boolean verifierGrilleAttaque(int **grille_attaque, Coordonnees attaque);

// Jeu
void jeu();
boolean victoire(Joueur j1, Joueur j2);

/*
char *saisirCible();
boolean verifierChaine(char *chaine);
*/

#endif