#ifndef SERVER_H
#define SERVER_H

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

// Joueurs
Joueur initialiserJoueur(int joueur);
void afficherStatistiquesJoueur(Joueur j1, Joueur j2);
int **initialiserGrille();
void afficherGrille(int **grille);
void afficherGrillesJeu(int **grille, int **grille_attaque);

void erreur(const char *message);
int receptionInt(int cli_sockfd);
void envoiMessageClient(int cli_sockfd, char *message);
void envoiIntClient(int cli_sockfd, int message);
void envoiMessage2Clients(int *cli_sockfd, char *message);
void envoiInt2Clients(int *cli_sockfd, int message);
void envoyerGrille(int **grille, int sockfd);
int **recevoirGrille(int sockfd);
void envoyerStatistiques(Joueur joueur, int sockfd);
void recevoirStatistiques(Joueur *joueur, int sockfd);

// Connexion
int definirListenerServeur(int port);
void recevoirClients(int lis_sockfd, int *cli_sockfd);

// Attaques des joueurs
Coordonnees attaqueJoueur(int cli_sockfd);
boolean verifierAttaque(int **grille_attaque, Coordonnees cible);
int **mettreAJourGrille(int **grille, Coordonnees cible, boolean attaque);
int **mettreAJourGrilleBateauTouche(int **grille, Coordonnees cible, int symbole);
void mettreAJourBateauxJoueur(Joueur *j, Coordonnees position, int *symbole);
int nombreAttaqueSurBateaux(boolean *degats, int longueur_bateau);
boolean verifierToucheCoule(Joueur *joueur, int symbole);
boolean verifierGrilleAttaque(int **grille_attaque, Coordonnees attaque);
boolean victoire(Joueur j1, Joueur j2);
void envoyerMiseAJour(int *cli_sockfd, int attaqueX, int attaqueY, int id_joueur);

// Thread de jeu
void *lancerJeu(void *donnees_thread);

#endif