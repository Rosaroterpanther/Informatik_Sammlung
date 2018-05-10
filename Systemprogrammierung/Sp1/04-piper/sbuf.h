
#ifndef SBUF_H
#define SBUF_H

#include "sem.h"

typedef struct sbuf sbuf;

sbuf* sbuf_create( int amount_sems );

int sbuf_add_sem(sbuf* buf, SEM* sem);

SEM* sbuf_get_sem(sbuf* buf, int id);

#endif /* SBUF_H */
