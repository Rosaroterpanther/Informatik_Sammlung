#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "halde.h"

#define SIZE (1024*1024*1)

int main(int argc, char *argv[]) {
	printList();

	char* m1 = malloc(200*1024);
	printList();
	char* m2 = malloc(200*1024);
	printList();
	char* m3 = malloc(200*1024);
	printList();
	char* m4 = malloc(200*1024);
	printList();

	free(m1);
	printList();
	free(m2);
	printList();
	free(m3);
	printList();
	free(m4);
	printList();


	// Randfall 1: size=0
	char* mzero = malloc(0);
	printList();
	free(mzero);
	printList();

	// Randfall 2: ENOMEM:12 Out of memory
	char* menomem = malloc(SIZE + 1);
	printList();
	free(menomem);
	printList();

	char* m1 = malloc(200*1024);
	printList();
	char* m2 = malloc(200*1024);
	printList();
	char* m3 = malloc(200*1024);
	printList();
	char* m4 = malloc(200*1024);
	printList();

	free(m1);
	printList();
	free(m2);
	printList();
	free(m3);
	printList();
	free(m4);
	printList();
	
	exit(EXIT_SUCCESS);
}
