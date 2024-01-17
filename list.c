/**
 * Austin Van Braeckel
 * 2020-10-08
 * A list ADT implemented using a linked list, with all of the functions outlined
 * in the assignment description.
 */

#include "list.h"

// BASIC FUNCTIONS

/**
 * Creates and returns a new Performance struct pointer
 * with memory already allocated, and all initializes all values to 0.
 */
struct Performance *newPerformance(){
    struct Performance *p = malloc(sizeof(struct Performance));
    p->frees = 0;
    p->mallocs = 0;
    p->reads = 0;
    p->writes = 0;
    return p;
}

/**
 * Adds the given source item, with a size of the given width, to the head (start) of the given list, 
 * and tracks any performance-related operations that are used in this process.
 */
void push(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width){
    // Create and allocate memory for a new node, and its data
    struct Node *new_node = malloc(sizeof(struct Node));
    if ( new_node == NULL ) {
        fprintf(stderr, "ERROR: memory allocation failed.\n");
        exit(-1);
    }
    new_node->data = malloc(width);
    if ( new_node->data == NULL ) {
        fprintf(stderr, "ERROR: memory allocation failed.\n");
        exit(-1);
    }

    memcpy(new_node->data, src, width);

    // Set current list head as the new_node's next, and the new node as the new head of the list
    new_node->next = (*list_ptr);
    *list_ptr = new_node;

    // Update performance
    performance->mallocs++;
    performance->writes++;
}

/**
 * Copies data from the head of the list to the given destination, with a size of the given width.
 * Exits with an error message if list is empty.
 */
void readHead(struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width){
    // Check if list is empty
    if ( isEmpty(performance, list_ptr) ) {
        fprintf(stderr, "ERROR: Cannot perform readHead - the list is empty.\n");
        exit(-1);
    }

    memcpy(dest, (*list_ptr)->data, width);

    // Update performance
    performance->reads++;
}

/**
 * Removes the item at the head of the list, and copies it in the given destination.
 */
void pop(struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width){
    // Check if list is empty
    if ( isEmpty(performance, list_ptr) ) {
        fprintf(stderr, "ERROR: Cannot perform pop - the list is empty.\n");
        exit(-1);
    }

    memcpy(dest, (*list_ptr)->data, width);

    // Remove and free head, then set new ehad
    struct Node *removed = (*list_ptr);
    struct Node *new_head = (*list_ptr)->next;
    free( removed->data );
    free( removed );
    *list_ptr = new_head;

    // Update performance
    performance->frees++;
    performance->reads++;
}

/**
 * Retrieves and returns a pointer to the pointer of the second item in the  given list.
 */
struct Node **next(struct Performance *performance, struct Node **list_ptr){
    // Check if list is empty
    if ( isEmpty(performance, list_ptr) ) {
        fprintf(stderr, "ERROR: Cannot perform next - the list is empty.\n");
        exit(-1);
    }

    // Update performance
    performance->reads++;

    return &((*list_ptr)->next);
}


/**
 * Checks if the given list is empty, returning a 1 if the list is empty,
 * and otherwise returning a 0.
 */
int isEmpty(struct Performance *performance, struct Node **list_ptr){
    if ( list_ptr == NULL || (*list_ptr) == NULL || (*list_ptr)->data == NULL ) {
        return 1;
    } else {
        return 0;
    }
} 

// DERIVED FUNCTIONS

/**
 * Will pop items off the given list until the list is empty.
 */
void freeList(struct Performance *performance, struct Node **list_ptr) {
    // removed data isn't being processed so only 1 byte is copied to dest for simplicity
    char dest[2];
    unsigned int width = 1;

    while ( !isEmpty(performance, list_ptr) ) { // Continues to pop items while the list has elements in it
        pop(performance, list_ptr, dest, width);
    }
}

/**
 * Retrieves the item at the specific index, with a size of the given width, and copies it to the given destination.
 */
void readItem(struct Performance *performance, struct Node **list_ptr, unsigned int index, void *dest, unsigned int width) {
    struct Node **next_node = list_ptr;
    int i;
    for ( i = 0; i < index; i++ ) {
        next_node = next(performance, next_node);
    }
    readHead(performance, next_node, dest, width);
}

/**
 * Adds an element to the end of the list.
 */
void appendItem(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width) {
    struct Node **next_node = list_ptr;
    while ( !isEmpty(performance, next_node) ) { // Continues through the list elements until it reaches the first empty node at the end of the list
        next_node = next(performance, next_node);
    }
    push(performance, next_node, src, width);
}

/**
 * Inserts the item from the specified source, with the given size, at the given index.
 */
void insertItem(struct Performance *performance, struct Node **list_ptr, unsigned int index, void *src, unsigned int width) {
    struct Node **next_node = list_ptr;
    int i;
    for ( i = 0; i < index; i++ ) { // Continues through the list elements until the specified index is reached
        next_node = next(performance, next_node);
    }
    push(performance, next_node, src, width);
}

/**
 * Inserts the given item, with a size of the given width, at position 0, adding to the beginning of the list.
 */
void prependItem(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width) {
    insertItem(performance, list_ptr, 0, src, width);
}

/**
 * Removes the item at the given index in the given list.
 */
void deleteItem(struct Performance *performance, struct Node **list_ptr, unsigned int index) {
    // removed data isn't being processed so only 1 byte is copied to dest for simplicity
    char dest[2];     
    unsigned int width = 1; 
    
    struct Node **next_node = list_ptr;
    int i;
    
    for ( i = 0; i < index; i++ ) { // Continues through the list elements until the specified index is reached
        next_node = next(performance, next_node);
    }
    pop(performance, next_node, dest, width);
}

/**
 * Finds a match of the given target in the given list (if it exists) using a linear search, returning its index in the list; if there is
 * no match to the target in the entire list, the function will return -1. The comparative ("compar") function will return a 0 if there
 * is a match, and a non-zero value otherwise. 
 */
int findItem(struct Performance *performance, struct Node **list_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width) {
    int index = 0;
    struct Node **next_node = list_ptr; 
    struct Node node[width];

    while ( !isEmpty(performance, next_node) ) {
        readHead(performance, next_node, node, width);
        if ( compar( node, target) == 0 ) { // Match
            return index;
        } // otherwise mismatch
        next_node = next(performance, next_node);
        index ++;
    }
    return -1; // No match in the list was found
}







