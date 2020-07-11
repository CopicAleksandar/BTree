#include "stdafx.h"

#include <stdio.h>

#include "btree.h"

#ifdef ROOT_GLOBAL
node_t *root_ptr = NULL;
#endif

void insert(node_t **root, int pid) {
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	new_node->pid = pid;
	new_node->left = NULL;
	new_node->right = NULL;

	while (*root) {
		if (pid < (*root)->pid)
			root = &(*root)->left;
		else
			root = &(*root)->right;
	}

	*root = new_node;
}

void insert_r(node_t **root, int pid) {
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	new_node->pid = pid;
	new_node->right = new_node->left = NULL;

	if (!(*root))
		*root = new_node;
	else if (pid < (*root)->pid)
		insert_r(&(*root)->left, pid);
	else // pid >= (*root)->pid
		insert_r(&(*root)->right, pid);
}

void delete_tree_r(node_t *root) {
	if (!root)
		return;

	delete_tree_r(root->left);
	delete_tree_r(root->right);

	free(root);
}

void print_tree_r(node_t *root) {
	if (root) {
		printf("%d\n", root->pid);
		print_tree_r(root->left);
		print_tree_r(root->right);
	}
}

node_t *find(node_t **root, int pid) {
	if (*root) {
		if ((*root)->pid == pid)
			return *root;
		if (pid < (*root)->pid)
			return find(&(*root)->left, pid);
		else
			return find(&(*root)->right, pid);
	}
	return NULL;
}