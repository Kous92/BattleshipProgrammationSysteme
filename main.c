#include "battleship.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	system("clear");

	Joueur j1 = initialiserJoueur(1);
	// afficherGrillesJeu(j1.grille, j1.grille_attaque);

	j1.grille = placerBateauxAleatoirement(j1.bateaux, j1.grille);
	afficherGrillesJeu(j1.grille, j1.grille_attaque);
	afficherStatistiquesJoueur(j1);

	definirCible();

	return 0;
}