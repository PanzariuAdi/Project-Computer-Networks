#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include "header.h"

#define BUFFSIZE 4096

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main (int argc, char *argv[])
{
  int sd, msgSize;;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  		// mesajul trimis
  int nr=0;
  char buf[10];

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
  {
    printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
    return -1;
  }

  port = atoi (argv[2]);

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror ("Eroare la socket().\n");
    return errno;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);
  
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
  {
    perror ("[client]Eroare la connect().\n");
    return errno;
  }

  /* citirea mesajului 
  printf ("[client]Introduceti un numar: ");
  fflush (stdout);
  read (0, buf, sizeof(buf));
  nr=atoi(buf);
  */
  //scanf("%d",&nr);
  
  //printf("[client] Am citit %d\n",nr);

  /* trimiterea mesajului la server 
  if (write (sd,&nr,sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
*/

  if (read (sd, &msgSize,sizeof(int)) < 0)
  {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
  }

  if (read (sd, &clientProblem,msgSize) < 0)
  {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
  }

  /* afisam mesajul primit */
  printf("%s\nI:%sO:%s\n", clientProblem.problemText, clientProblem.problemInput, clientProblem.problemOutput);

  char buffer[BUFFSIZE], sursa[50];

  printf("Dati numele fisierului sursa : ");
  
  scanf("%s", sursa);



  FILE * clientCode;
  clientCode = fopen(sursa, "r");

  if(clientCode == NULL) { 
      strcpy(sursa, "Fisier invalid");
      write(sd, "Invalid", sizeof("Invalid"));
      return errno;      
  }

  if (write (sd, sursa, sizeof(sursa)) < 0)
  {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
  }


  while(fgets(buffer, 255, (FILE*)clientCode) && clientCode != NULL) {
        write(sd, buffer, sizeof(buffer));
  }
  
  /* inchidem conexiunea, am terminat */
  close (sd);
}

