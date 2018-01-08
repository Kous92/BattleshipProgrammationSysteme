#include "battleship.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	system("clear");

	int **grille = initialiserGrille();
	afficherGrille(grille);
	
	Bateau *bateaux = (Bateau *) malloc(5 *sizeof(Bateau));

	bateaux[0].longueur_bateau = 5;
	bateaux[0].symbole = 1;
	bateaux[1].longueur_bateau = 4;
	bateaux[1].symbole = 2;
	bateaux[2].longueur_bateau = 3;
	bateaux[2].symbole = 3;
	bateaux[3].longueur_bateau = 3;
	bateaux[3].symbole = 3;
	bateaux[4].longueur_bateau = 2;
	bateaux[4].symbole = 4;
	
	grille = placerBateauxAleatoirement(bateaux, grille);
	printf("\n");
	afficherGrille(grille);

	free(bateaux);

	return 0;
}