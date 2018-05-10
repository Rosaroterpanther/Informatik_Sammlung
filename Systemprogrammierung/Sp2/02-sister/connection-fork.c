/*
  Verbindungs modul
*/

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "request.h"
#include "connection.h"

int initConnectionHandler(void){
  // Zombies einsammeln mit Hilfe von Signalen
  struct sigaction action;
	action.sa_handler = SIG_DFL;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_NOCLDWAIT; // SA_RESTART is not needed with SIG_DFL
	sigaction(SIGCHLD, &action, NULL);
	return initRequestHandler();
}

void handleConnection(int clientSock, int listenSock){
  pid_t pid = fork();
  if(pid == -1){
    perror("Fehler fork");
    exit (EXIT_FAILURE);
  } else if(pid == 0){
    // Kind prozess
    close(listenSock);
    FILE *client = fdopen(clientSock, "a+");
    if(client == NULL){
      perror("Fehler fdopen");
      exit(EXIT_FAILURE);
    }
    handleRequest(client);
    fclose(client);
    exit(EXIT_SUCCESS);
  } else{
    close(clientSock);
  }
}
