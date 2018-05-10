#include <stdio.h>
#include <stdlib.h>

#include "sem.h"
#include "sbuf.h"

struct sbuf{
    SEM** sems;
    int size;
};



sbuf* sbuf_create( int amount_sems ){
  sbuf *sbuf = malloc(sizeof(sbuf));
  if(sbuf == NULL){
    // TODO
    return NULL;
  }

  sbuf->sems = malloc( amount_sems * sizeof(SEM*));
  if(sbuf->sems == NULL){
    free(sbuf);
    // TODO
    return NULL;
  }

  sbuf->size = 0;
  return sbuf;
}

int sbuf_add_sem(sbuf* buf, SEM* sem){
  if(buf == NULL || sem == NULL){
    return -1;
  }
  int id = buf->size;
  buf->sems[id] = sem;
  buf->size += 1;

  return id;
}

SEM* sbuf_get_sem(sbuf* buf, int id){
  return buf->sems[id];
}
