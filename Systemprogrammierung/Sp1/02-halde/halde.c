#include "halde.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>

/// Magic value for occupied memory chunks.
#define MAGIC ((void*)0xbaadf00d)

/// Size of the heap (in bytes).
#define SIZE (1024*1024*1)

/// Memory-chunk structure.
typedef struct mblock {
	struct mblock *next;
	size_t size;
	char memory[];
} mblock;

/// Heap-memory area.
static char memory[SIZE];

/// Pointer to the first element of the free-memory list.
static struct mblock *head;

/// Value to check if halde is initialized 0 false, 1 true
static int initialized = 0;

/// Helper function to visualise the current state of the free-memory list.
void printList(void) {
	struct mblock* lauf = head;

	// Empty list
	if ( head == NULL ) {
		fprintf(stderr, "(empty)\n");
		return;
	}

	// Print each element in the list
	while ( lauf ) {
		fprintf(stderr, "(addr: 0x%08zx, off: %7zu, ", (uintptr_t) lauf, (uintptr_t)lauf - (uintptr_t)memory);
		fflush(stderr);
		fprintf(stderr, "size: %7zu)", lauf->size);
		fflush(stderr);

		if ( lauf->next != NULL ) {
			fprintf(stderr, "  -->  ");
			fflush(stderr);
		}
		lauf = lauf->next;
	}
	fprintf(stderr, "\n");
	fflush(stderr);
}

void *malloc (size_t size) {

	if(initialized == 0){
		 // Initialize the first element of the free-memory list
		 head = (mblock *) memory;
		 head->size = sizeof(memory) - sizeof(mblock);
		 head->next = NULL;
		 initialized = 1;
	}

	if(size == 0){
		return NULL;
	}

	mblock *walkthrough = head;
	mblock **prev_next = &head;

	// Walk through the free-memory list an search for a mblock with enough space
	while(walkthrough != NULL && walkthrough->size < size){
		prev_next = &(walkthrough->next);
		walkthrough = *prev_next;
	}

	if(walkthrough == NULL){
		// No mblock has enough space
		errno = ENOMEM;
		return NULL;
	}

	// Test if its worth it to create a new mblock
	if((walkthrough->size - size) > sizeof(mblock)){
		// Worth it create a new mblock and initialize
		mblock* new = (mblock *) (walkthrough->memory + size);
		new->size = walkthrough->size - sizeof(mblock) - size;
		new->next = walkthrough->next;

		// Edit size of current mblock
		walkthrough->size = size;

		// Add new mblock to free-memory list and leave walkthrough mblock out
		*prev_next = new;
	}else{
		// not worth it remove walkthrough mblock from the free-memory list
		*prev_next = walkthrough->next;
	}

	// set mblock to inactive
	walkthrough->next = MAGIC;

	return walkthrough->memory;
}

void free (void *ptr) {
	if(ptr == NULL){
		return;
	}

	// Create pointer on the mblock of the element that has to be free
	mblock *ptr_block = (mblock *) ptr - 1;

	if(ptr_block->next != MAGIC){
		// Ungueltiger pointer no used mblock
		abort();
	}

	// Add mblock on the beginning of the free-memory list
	ptr_block->next = head;
	head = ptr_block;

	return;
}

void *realloc (void *ptr, size_t size) {

	if(ptr == NULL){
		return malloc(size);
	}

	if(size <= 0){
		free(ptr);
		return NULL;
	}

	mblock *new = malloc(size);

	if(new != NULL){
		mblock *temp = (mblock *) ptr - 1;
		size_t copy_size;
		if(size < temp->size){
			copy_size = size;
		}else{
			copy_size = temp->size;
		}

		memcpy(new, ptr, copy_size);
		free(ptr);
	}

	return new;
}

void *calloc (size_t nmemb, size_t size) {
	size_t bytes = nmemb * size;
	void *new = malloc(bytes);

	//If nmemb or size is 0 malloc returns NULL and calloc returns NULL

	if(new != NULL){
		// Set the memory (bytes) behind new to 0
		memset(new, 0, bytes);
	}

	return new;
}
