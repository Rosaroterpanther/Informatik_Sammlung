#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define SERVER "faui03.cs.fau.de "
#define PORT "25"

char* adress;
int is_subject; // 0 false, 1 true (if subject is set)
char* subject;

int main(int argc, char const *argv[]) {

  if(argc == 2){
    is_subject = 0;
    subject = NULL;
    adress = (char* ) argv[1];
  }else if (argc == 4 && (strcmp(argv[1], "-s") == 0)){
    is_subject = 1;
    subject = (char* ) argv[2];
    adress = (char* ) argv[3];
  }else{
    fprintf(stderr, "Usage: snail [-s <subject>] <adress> \n");
    exit(EXIT_FAILURE);
  }

  // printf("Adress: %s, Subject: %s\n", adress, subject);
  // DNS Anfrage
  // Prepare Hints
  struct addrinfo *hints;

  memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG;

  //Make DNS Request
  struct addrinfo *head;
  int s = getaddrinfo(SERVER, PORT, &hints, &head);
  if(s != 0){
    fprintf(stderr, "Fehler getaddrinfo %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  struct addrinfo *rp;
  int sfd;
  for (rp=head; rp != NULL; rp=rp->ai_next){
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if(sfd == -1){
      continue;
    }else if(bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0){
      break;
    }else{
      close(sfd);
    }
  }

  freeaddrinfo(head);

  return 0;
}
