#include "battleship.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	system("clear");

	Coordonnees attaque;
	boolean touche = FALSE;

	int i = 0;
	jeu();
	// afficherGrillesJeu(j1.grille, j1.grille_attaque);

	// j1.grille = placerBateauxAleatoirement(j1.bateaux, j1.grille);
	// afficherGrillesJeu(j1.grille, j1.grille_attaque);
	// afficherStatistiquesJoueur(j1);

	/*
	while (i < 3)
	{
		attaque = definirCible();
		i++;

		touche = verifierAttaque(j1.grille, attaque);
		j1.grille_attaque = mettreAJourGrille(j1.grille_attaque, attaque, touche);

		afficherGrillesJeu(j1.grille, j1.grille_attaque);
	}
	*/

	return 0;
}