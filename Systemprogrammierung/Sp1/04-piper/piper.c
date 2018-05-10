#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "sem.h"
#include "sbuf.h"

#define LINE_LEN 100

static sbuf* sem_array;
static char line[LINE_LEN];

struct param{
    SEM* sem;
    char *pipe;
    FILE *desc;
};

void* writeThread(void* args);

int main(int argc, char const *argv[]) {
  /* piper <pipes …> */
  if(argc < 2){
    fprintf(stderr, "Fehler beim Aufruf %s <pipes…>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int amount_pipes = argc-1;

  // Semaphoren erstellen
  sem_array = sbuf_create(amount_pipes);
  if(sem_array == NULL){
    perror("Fehler sbuf_create");
    exit(EXIT_FAILURE);
  }

  for(int i=0; i < amount_pipes; i++){
    SEM *sem_temp = semCreate(0);
    sbuf_add_sem(sem_array, sem_temp);
  }

  // Prepare Parameteruebergabe
  struct param args[amount_pipes];

  // Open pipes
  for(int i =0; i < amount_pipes; i++){
    args[i].sem = sbuf_get_sem(sem_array, i);
    args[i].pipe = (char *) argv[i];
    args[i].desc = fopen(args[i].pipe, "w");
    if(args[i].desc == NULL){
      perror("Fehler open");
      exit(EXIT_FAILURE);
    }
  }

  // Threads erzeugen
  pthread_t write_threads[amount_pipes];

  for (int i=0; i < amount_pipes; i++){
    int error = pthread_create(&write_threads[i], NULL, writeThread, &args[i]);
    if(error != 0){
      errno = error;
      perror("Fehler pthread_create");
      exit(EXIT_FAILURE);
    }

  }

  while(1){
    // Zeilenweise von stdin lesen
    if(fgets(line, LINE_LEN, stdin) == NULL){
      if(ferror(stdin)){
        perror("Fehler fgets");
        exit(EXIT_FAILURE);
      }else{
        break;
      }
    }
    // An alle UNIX-Pipes weiter geben
    for(int i=0; i < amount_pipes; i++){
      V(sbuf_get_sem(sem_array, i));

      if(fputs(line,args[i].desc) < 0){
        perror("Fehler fputs");
        exit(EXIT_FAILURE);
      }
    }

    // Auf alle threads warten
    for(int i=0; i < amount_pipes; i++){
      P(sbuf_get_sem(sem_array, i));
    }
  }

  // Join threads
  for(int i=0; i < amount_pipes; i++){
    pthread_join(write_threads[i], NULL);
  }

  return EXIT_SUCCESS;
}

void* writeThread(void* args){
  struct param *arg = (struct param *) args;

  open(arg->pipe, O_RDONLY);
  char line[LINE_LEN];

  while(1){
    P(arg->sem);
    // fprintf(stderr, "Thread [%lu]:", pthread_self());
    if(fgets(line, LINE_LEN, arg->desc) == NULL){
      perror("Fehler fgets");
      return NULL;
    }
    printf("Thread %s\n", line);
    fflush(arg->desc);
    V(arg->sem);
  }
  return NULL;
}
