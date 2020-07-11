#pragma once

/*
* Binary Search Tree (BST)
*
* Supported operations:
*      insertion
*      deletion
*      traversal
*      search -- binary or linear
*/
 //#define ROOT_GLOBAL

#include <stdlib.h>

typedef struct node_t {
	int pid;
	struct node_t *left;
	struct node_t *right;
} node_t;

#ifdef ROOT_GLOBAL
extern node_t *root_ptr;
#define INSERT(pid) insert(&root_ptr, pid)
#define INSERT_R(pid) insert_r(&root_ptr, pid)
#define PRINT() print_tree_r(root_ptr)
#endif

/* Inserts new node into the tree. */
void insert(node_t **root, int pid);

/* Recursively insert new node. */
void insert_r(node_t **root, int pid);

void delete_tree_r(node_t *root);

/* Finds the node with specific pid and returns it. */
node_t *find(node_t **root, int pid);

/* Finds the node with specific value using linear(sequential) search. */
node_t *find_linear(node_t *root, int value);

/* Removes a node with id from the tree */
node_t *remove_node(node_t **root, int id);

/* Prints the tree in nice format using recursion. */
void print_tree_r(node_t *root);

/* Prints the tree in nice format WITHOUT using recursion. */
void print_tree(node_t *root);