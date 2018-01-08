#include "battleship.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	system("clear");

	int **grille = initialiserGrille();
	afficherGrille(grille);
	
	return 0;
}