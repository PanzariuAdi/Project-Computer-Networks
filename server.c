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
#include "configurare.h"
#include <time.h>

#define PORT 2909
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
int compile();
void evaluate(char sursa[], int problemID, int studentID);
int compareFiles(char file1[], char file2[]);
void removeCommand(char folder[], char file[]);
void clasamentSort();
void updateScore(int id, int value);

static void *exit_server_f(void * a) {
    sleep(20);
    exit(0);
}

int currentStudent = -1, contor;
int bonus[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}, contor_bonus;

int main (int argc, char * argv[])
{
    pthread_t exit_server;
    pthread_create(&exit_server, NULL, &exit_server_f, NULL);

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

    for(int i = 0; i < NR_PROBLEMS; i++) {
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
    currentStudent++;
    int nr, i=0;
	  struct thData tdL; 
	  tdL= *((struct thData*)arg);
	
    int randNumber = rand() % NR_PROBLEMS;
    serverProblem = problems[randNumber];

    clasamentFinal[currentStudent].idProblem = randNumber;
    clasamentFinal[currentStudent].idStudent = currentStudent;
    clasamentFinal[currentStudent].punctaj = 0;

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
    
    char buffer[BUFFSIZE], sursa[50], fullPath[80];
    int msgsize = 0;
    FILE * clientCode;
    
    int begin_time = time(NULL);

    if (read (tdL.cl, &sursa, sizeof(sursa)) <= 0) {
		  printf("[Thread %d] ",tdL.idThread);
		  perror ("[Thread]Eroare la read() catre client.\n");
		}

    if(!strcmp(sursa, "Invalid")) return;

    int end_time = time(NULL);
    int problemTime;
    switch (randNumber)
    {
    case 0:
      problemTime = MAX_P00_TIME;
    break;

    case 1:
      problemTime = MAX_P01_TIME;
    break;

    case 2:
      problemTime = MAX_P02_TIME;
    break;

    case 3:
      problemTime = MAX_P03_TIME;
    break;

    case 4:
      problemTime = MAX_P04_TIME;
    break;

    case 5:
      problemTime = MAX_P05_TIME;
    break;

    default:
      break;
    }

    if((end_time - begin_time) >= problemTime) {
       printf("GATAAAAAAAAAAAAAAAAAAA\n");
       return;
    }

    strcpy(fullPath, "./rezolvari/");
    strcat(fullPath, sursa);
    clientCode = fopen(fullPath, "w");

    //printf("Sursa : %s\n", sursa);

    while(read(tdL.cl, buffer, sizeof(buffer))) {
        //printf("%s", buffer);
        fprintf(clientCode, "%s", buffer);
    } 
    fclose(clientCode);

    evaluate(sursa, randNumber, currentStudent);


    clasamentSort();

    for(int i = 0; i < currentStudent + 1; i++) {
        printf("Nume : %s Punctaj : %d\n", clasamentFinal[i].studentName, clasamentFinal[i].punctaj);
    }

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

int compile(char sursa[]) {
    char dest[80];
    int size = strlen(sursa);
    strcpy(dest, sursa);
    dest[size - 2] = '\0';
    char command[250];
    strcpy(command, "cd rezolvari/ && ");
    strcat(command, "gcc ");
    strcat(command, sursa);
    strcat(command, " -o ");
    strcat(command, dest);
    strcat(command, " && ./");
    strcat(command, dest);  
    //printf("compile --> %s\n", command);  
    int r = system(command);
    return r;   
}

int execute(char comanda[]) {
    char command[300];
    strcpy(command, comanda);
    int r = system(command);
    return r;
}

void evaluate(char sursa[], int problemID, int studentID) {
    char problemInput[50], problemOutput[50], clientIn[50], clientOut[50], cpCommand[250], testCase[2];
    int result;

    problemInput[0] = '0';
    problemOutput[0] = '0';
    problemInput[1] = problemID + '0';
    problemOutput[1] = problemID + '0';


    for(int i = 0; i <= currentStudent; i++) {
        if(clasamentFinal[i].idStudent == studentID){
            char tmp[50];
            strcpy(tmp, sursa);
            tmp[strlen(tmp) - 2] = '\0';
            strcpy(clasamentFinal[i].studentName, tmp);
            break;
        }
    }
    
    for(int i = 1; i <= NR_TESTS; i++) {
        strcpy(problemInput + 2, "");
        strcpy(problemOutput + 2, "");

        strcat(problemInput, "_");
        strcat(problemOutput, "_");
        testCase[0] = i + '0';
        strcat(problemInput, testCase);
        strcat(problemOutput, testCase);
        strcat(problemInput, ".in");
        strcat(problemOutput, ".out");

        strcpy(clientIn, sursa);
        strcpy(clientOut, sursa);
        int size = strlen(sursa);

        clientIn[size - 1] = 'i';
        clientIn[size] = 'n';

        clientOut[size - 1] = 'o';
        clientOut[size] = 'u';
        clientOut[size + 1] = 't';

        strcpy(cpCommand, "cd IO_Probleme/ ; ");
        strcat(cpCommand, "cp ");
        strcat(cpCommand, problemInput);
        strcat(cpCommand, " ../rezolvari/");
        execute(cpCommand);

        strcpy(cpCommand, "cd IO_Probleme/ ; ");
        strcat(cpCommand, "cp ");
        strcat(cpCommand, problemOutput);
        strcat(cpCommand, " ../rezolvari/");
        execute(cpCommand);

        strcpy(cpCommand, "cd rezolvari/ ; ");
        strcat(cpCommand, "mv ");
        strcat(cpCommand, problemInput);
        strcat(cpCommand, " ");
        strcat(cpCommand, clientIn);
        execute(cpCommand);

        time_t begin = clock();
        compile(sursa);
        time_t end = clock();

        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

        //printf("Timp problema : %f\n", time_spent);
        /*
        strcpy(cpCommand, "cd rezolvari/ ; ");
        strcat(cpCommand, "touch ");
        strcat(cpCommand, problemOutput);
        strcat(cpCommand, " ");
        strcat(cpCommand, clientOut);
        execute(cpCommand);  */

        
        result = compareFiles(problemOutput, clientOut);
      
        if(result == 1) {
            //printf("Corect! !\n");
            updateScore(studentID, 25);

            switch (problemID)
            {
            case 0:
              if(time_spent < P00_time)
                updateScore(studentID, 5);
            break;

            case 1:
              if(time_spent < P01_time)
                updateScore(studentID, 5);
            break;

            case 2:
              if(time_spent < P02_time)
                updateScore(studentID, 5);
            break;

            case 3:
              if(time_spent < P03_time)
                updateScore(studentID, 5);
            break;

            case 4:
              if(time_spent < P04_time)
                updateScore(studentID, 5);
            break;

            case 5:
              if(time_spent < P05_time)
                updateScore(studentID, 5);
            break;
            
            default:
              break;
            }

        } else {
            //printf("Gresit!\n");
        }
        
        removeCommand("rezolvari/", problemOutput);
        removeCommand("rezolvari/", clientOut);
        removeCommand("rezolvari/", clientIn); 
    }   
}

int compareFiles(char file1[], char file2[]) {
    char path1[80], path2[80], buff1[256], buff2[256], rmCommand[100];
    FILE *pfile1;
    FILE *pfile2;

    strcpy(path1, "./rezolvari/");
    strcpy(path2, "./rezolvari/");
    strcat(path1, file1);
    strcat(path2, file2);

    //printf("\n%s\n%s\n", path1, path2);

    if( (pfile1 = fopen(path1, "r")) == NULL) {
        return 0;
    } else {
        if( fgets(buff1, 255, pfile1) != NULL) {
            puts(buff1);
        }
    }
    fclose(pfile1);

    if( (pfile2 = fopen(path2, "r")) == NULL) {
        return 0;
    } else {
        if( fgets(buff2, 255, pfile2) != NULL) {
            puts(buff2);
        }
    }
    fclose(pfile2);

    if(buff1[strlen(buff1) - 1] == '\n') buff1[strlen(buff1) - 1] = '\0';
    if(buff2[strlen(buff2) - 1] == '\n') buff2[strlen(buff2) - 1] = '\0'; 

    if(!strcmp(buff1, buff2)) return 1;
    return 0;
}

void removeCommand(char folder[], char file[]) {
    char command[100];
    strcpy(command, "cd ");
    strcat(command, folder);
    strcat(command, " ; rm ");
    strcat(command, file);
    execute(command);
}

void clasamentSort() {
    int ok  = 0;
    while(!ok) {
        ok = 1;
        for(int i = 0; i < currentStudent; i++) {
            if(clasamentFinal[i].punctaj < clasamentFinal[i + 1].punctaj) {
                struct Clasament aux;
                aux = clasamentFinal[i];
                clasamentFinal[i] = clasamentFinal[i + 1];
                clasamentFinal[i + 1] = aux;
                ok = 0; 
            }
        }
    }
}

void updateScore(int id, int value) {
    for(int i = 0; i <= currentStudent; i++) {
        if(clasamentFinal[i].idStudent == id) {
            clasamentFinal[i].punctaj += value;
            break;
        }

    }
}
