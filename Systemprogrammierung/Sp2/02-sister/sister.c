#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "cmdline.h"
#include "connection.h"

#define DEFAULT_PORT 2016

char* wwwpath;
int port = 2016;

int main(int argc, char const *argv[]) {

  if(cmdlineInit(argc, argv) != 0){
    fprintf(stderr, "Usage: sister --wwwpath=<dir> [--port=<p>] \n");
    exit(EXIT_FAILURE);
  }else if(argc == 3){
    port = itao(cmdlineGetValueForKey("port"));
  }


  // Init Funktion connection-fork.c
  if(initConnectionHandler() != 0){
    // TODO
  }

  // TCP/IPv6-Socket erzeugen
  int listener = socket(PF_INET6, SOCK_STREAM, 0);
  if(listener == -1){
    perror("Fehler socket");
    exit("EXIT_FAILURE");
  }

  // Sofortiges Wiederverwendung des Ports forcieren
  int flag = 1;
  if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1){
    perror("Fehler setsockopt");
    exit(EXIT_FAILURE);
  }

  // Adress Struktur anlegen
  struct sockaddr_in6 adress;
  memset(&adress, 0, sizeof(adress));
  adress.sin6_family = AF_INET6;
  adress.sin6_port_t = htons(port);
  adress.sin6_addr = in6_addr_any;

  // Socket binden
  if(bind(listener, (const struct sockaddr *) adress, sizeof(adress)) == -1){
    perror("Fehler bind");
    exit(EXIT_FAILURE);
  }

  // Verbindungsannahme vorbereiten
  if(listen(listener, SOMAXCONN) == -1){
    perror("Fehler listen");
    exit(EXIT_FAILURE);
  }

  // Anfragen behandeln
  for(;;){
    int clientSock = accept(listener, NULL, NULL);
    if(clientSock == -1){
      perror("Fehler accept");
      exit(EXIT_FAILURE);
    }
    handleConnection(clientSock, listener);
  }

  return 0;
}
