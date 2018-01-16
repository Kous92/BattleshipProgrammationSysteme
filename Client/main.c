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
	jeu();

	return 0;
}