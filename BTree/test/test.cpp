// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "../libbtree/btree.h"
#include "test.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
#pragma comment(lib, "../Debug/libbtree.lib")
#else
#pragma comment(lib, "../Release/libbtree.lib")
#endif

void handle_request(int a, int b, int c);

int foo(int a, int b, HANDLE handle, int c) {
	handle(a, b, c);
	return a;
}

int main()
{
	// node_t *root = NULL;
	// insert(&root, 4);
	//INSERT(4);

	//PRINT();

	// foo(3, 4, handle_request, 5);

	/*
	node_t *root = NULL;
	insert(&root, 5);
	insert(&root, 4);
	insert(&root, 6);
	insert(&root, 3);
	insert(&root, 7);
	insert(&root, 1);
	insert(&root, 8);

	node_t *node = find(&root, 7);
	printf("FOUND %d\n", node->pid);
	*/
	char str[3] = "10";
	int a = atoi("10");
	printf("a= %d\nstr= %d\n", a, atoi(str));
	
	return 0;
}

void handle_request(int a, int b, int c) {
	printf("%d %d %d\n", a, b, c);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
