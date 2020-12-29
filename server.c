#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "header.h"

#define PORT 2908
#define BUFFSIZE 4096

extern int errno;

typedef struct thData{
	int idThread; 
	int cl;
}thData;

static void *treat(void *);
void raspunde(void *);
void initializeProblems();
void readIO(int nr, char in[], char out[]);

int main ()
{
    struct sockaddr_in server;	
    struct sockaddr_in from;	 
    int sd;		//descriptorul de socket 
    int pid;
    pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
    int i=0;
    
    initializeProblems();

    /* crearea unui socket */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[server]Eroare la socket().\n");
        return errno;
    }

    int on=1;
    setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    
    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));
    
    server.sin_family = AF_INET;	
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);
    
    
    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1){
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

    if (listen (sd, 2) == -1) {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

    while (1) {
      int client;
      thData * td; //parametru functia executata de thread     
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

        // client= malloc(sizeof(int));
        /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0) {
        perror ("[server]Eroare la accept().\n");
        continue;
      }
    
          /* s-a realizat conexiunea, se astepta mesajul */
      
    // int idThread; //id-ul threadului
    // int cl; //descriptorul intors de accept

      td=(struct thData*)malloc(sizeof(struct thData));	
      td->idThread=i++;
      td->cl=client;

      pthread_create(&th[i], NULL, &treat, td);	      
          
    }//while    
};


void initializeProblems() {
    FILE * fp;
    char buff[1000];

    fp = fopen("cerinte.txt", "r");

    for(int i = 0; i < 3; i++) {
        fgets(buff, 1000, (FILE*)fp);
        buff[strlen(buff) - 1] = '\0';
        problems[i].problemID = i;
        strcpy(problems[i].problemText, buff);
    }
}

static void *treat(void * arg)
{		
		struct thData tdL; 
		tdL= *((struct thData*)arg);	
		//printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
		//fflush (stdout);		 
		pthread_detach(pthread_self());		
		raspunde((struct thData*)arg);
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
		return(NULL);	
  		
};


void raspunde(void *arg)
{
    int nr, i=0;
	  struct thData tdL; 
	  tdL= *((struct thData*)arg);
	
    int randNumber = rand() % 3;
    serverProblem = problems[randNumber];

    printf("%d\n", randNumber);

    readIO(randNumber, serverProblem.problemInput, serverProblem.problemOutput);

    int structSize = sizeof(serverProblem);

    if (write (tdL.cl, &structSize, sizeof(int)) <= 0) {
		  printf("[Thread %d] ",tdL.idThread);
		  perror ("[Thread]Eroare la write() catre client.\n");
		} 

    if (write (tdL.cl, &serverProblem, structSize) <= 0) {
		  printf("[Thread %d] ",tdL.idThread);
		  perror ("[Thread]Eroare la write() catre client.\n");
		}
    
    char buffer[BUFFSIZE];
    int msgsize = 0;
    FILE * clientCode;
    clientCode = fopen("abc.c", "a");

    while(read(tdL.cl, buffer, sizeof(buffer))) {
        fprintf(clientCode, "%s", buffer);
    }    
    fclose(clientCode);
    
  /*
    if (read (tdL.cl, &nr,sizeof(int)) <= 0){
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");
		}
	
	  printf ("[Thread %d]Mesajul a fost receptionat...%d\n",tdL.idThread, nr);
		      
		pregatim mesajul de raspuns 
		nr++;      
	  printf("[Thread %d]Trimitem mesajul inapoi...%d\n",tdL.idThread, nr);
		*/    
		      /* returnam mesajul clientului */

}

void readIO(int nr, char in[], char out[]) {
    FILE * input;
    FILE * output;
    
    char fileName[30], fullPath[100];;
    strcpy(fileName, "");
    strcpy(fullPath, "");

    if(nr < 10) {
        fileName[0] = '0';
        fileName[1] = nr + '0';
    } else {
        fileName[0] = (nr / 10) + '0';
        fileName[1] = (nr % 10) + '0';
    }

    strcpy(fileName + 2, "_1.in");
    strcat(fullPath, "./IO_Probleme/");
    int basename = strlen(fullPath);
    strcat(fullPath, fileName);
    input = fopen(fullPath, "r");
    //printf("I: %s\n", fullPath);

    strcpy(fileName + 2, "_1.out");
    strcpy(fullPath + basename, fileName);
    //printf("O: %s\n", fullPath);
    output = fopen(fullPath, "r");

    fgets(in, 100, (FILE*)input);
    fgets(out, 100, (FILE*)output);

    fclose(input);
    fclose(output);
}