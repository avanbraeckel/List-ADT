/**
 * Austin Van Braeckel
 * 2020-10-08
 * A header file containing all necessary declarations, includes, and macro definitions
 */

#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// List node
struct Node {
  void *data;
  struct Node *next;
};

// Tracks the performance of the list
struct Performance {
  unsigned int reads;
  unsigned int writes;
  unsigned int mallocs;
  unsigned int frees;
};

// Function prototypes
struct Performance *newPerformance();
void push(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width);
void readHead(struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width);
void pop(struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width);
struct Node **next(struct Performance *performance, struct Node **list_ptr);
int isEmpty(struct Performance *performance, struct Node **list_ptr);

// Derived function prototypes
void freeList(struct Performance *performance, struct Node **list_ptr);
void readItem(struct Performance *performance, struct Node **list_ptr, unsigned int index, void *dest, unsigned int width);
void appendItem(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width);
void insertItem(struct Performance *performance, struct Node **list_ptr, unsigned int index, void *src, unsigned int width);
void prependItem(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width);
void deleteItem(struct Performance *performance, struct Node **list_ptr, unsigned int index);

int findItem(struct Performance *performance, struct Node **list_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width);

#endif