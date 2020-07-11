#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>

#include "../libbtree/btree.h"

#pragma comment(lib, "Ws2_32.lib")

/* typedef for handle function*/
typedef void (*HANDLE_R)(SOCKET, char *, int, int, node_t **root);

typedef struct params_t {
  SOCKET socket;
  bool RECV_THREAD;
  HANDLE_R request_handler;
  node_t **root;
} params_t;

bool init_windows_sockets();

SOCKET init_client();

SOCKET init_server();

/*
   function template:
   handle(char *buffer, int size, int request_code, int argc, void **params);
   params -- array of pointers to optional parameters
   

   to avoid using optional parameters, set argc to 0, and params to NULL
*/
// void server_listen(SOCKET socket, WAIT_MODE mode,
//                 void (*f)(char *, int, int, int, void **), int argc,
// void **argv, ...);

void server_listen(SOCKET socket, bool RECV_THREAD, HANDLE_R request_handler,
                   node_t **root);

HANDLE create_thread(params_t *params);

DWORD WINAPI thread(LPVOID lpParams);

int send(SOCKET socket, char *buffer, int *size);

int receive(SOCKET socket, char *buffer);

int send_data(SOCKET, char *buffer, int size);

int receive_data(SOCKET socket, char *buf, int size);

void cleanup(SOCKET socket);
