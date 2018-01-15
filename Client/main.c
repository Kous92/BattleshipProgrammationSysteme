#include "battleship.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define DEBUG

int main(int argc, char *argv[])
{
	system("clear");

	Coordonnees attaque;
	boolean touche = FALSE;
	char *adresse_ip = "127.0.0.1";
	int port = 8888;

	int i = 0;
	jeu();

	return 0;
}