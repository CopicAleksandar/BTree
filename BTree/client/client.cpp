// client.cpp : This file contains the 'main' function. Program execution begins
// and ends there.
//

#include "pch.h"

#include "../socket/socket.h"
#include <iostream>

#pragma comment(lib, "../Debug/socket.lib")

void input(char *buffer, const char *message, int size);
void clear_buffer(char *buffer);
void print(char *buffer, int size, int request_code, int argc, void **params);

int main() {
  SOCKET socket = init_client();
  int size, request_code = 2;

  /*
  char *buffer = (char *)malloc(20);

  printf("How many messeges will be sent?");
  input(buffer, "\n", 5);
  int quantity = atoi(buffer);
  size = 4;
  send(socket, (char *)(&quantity), &size);

  for (int i = 0; i < quantity; ++i) {
    input(buffer, "\nEnter message: ", 20);
    size = strlen(buffer);
    client_send(socket, buffer, &size);
  }
  */

  char buffer[20];
  char *msg = (char *)malloc(4);

    size = 4;
    send(socket, (char *)(&request_code), &size);
  for (int i = 0; i < 3; ++i) {
    /* Connect to server. */
    //socket = init_client();

    /* Send request_code. */
    //size = 4;
    //send(socket, (char *)(&request_code), &size);

    input(msg, "\nEnter pid: ", 4);
	size = strlen(msg) + 1;
	//int pid = atoi(msg);
    send(socket, msg, &size);

	receive(socket, buffer);
	printf("%s\n", buffer);

	// SOCKET socket_server = init_server();
	// server_listen(socket_server, NO_LOOP, print, 0, NULL);
  }

  free(msg);
  cleanup(socket);
}

void print(char *buffer, int size, int request_code, int argc, void **params) {
    printf("%s\n", buffer);
}

void input(char *buffer, const char *message, int size) {
  printf("%s", message);
  fgets(buffer, size, stdin);
  clear_buffer(buffer);
}

void clear_buffer(char *buffer) {
  char *c;
  c = strchr(buffer, '\n');

  if (!c)
    while (getchar() != '\n')
      ;
  else
    *c = '\0';
}