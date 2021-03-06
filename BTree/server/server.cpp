// server.cpp : This file contains the 'main' function. Program execution begins
// and ends there.
//

#include "pch.h"

//#include "../libbtree/btree.h"
#include "../socket/socket.h"

#include <conio.h>
#include <stdio.h>

#define DEBUG

/* Linker flags */
#ifdef DEBUG
#pragma comment(lib, "../Debug/socket.lib")
#pragma comment(lib, "../Debug/libbtree.lib")
#else
#pragma comment(lib, "../Release/socket.lib")
#pragma comment(lib, "../Release/libbtree.lib")
#endif

enum REQUEST_CODE { PID = 0, VALUE, SIGN_IN };

void handle_request(SOCKET socket, char *buffer, int size, int request_code,
                    node_t **root);

void send_response(SOCKET socket, char *buffer, int buffer_len, int response);

HANDLE create_listen_thread(params_t *params);

DWORD WINAPI listen_thread(LPVOID lpParams);

params_t *pack_params(SOCKET socket, bool RECV_THREAD,
                      HANDLE_R request_handler, node_t **root);

int main() {
  node_t *root = NULL;
  SOCKET socket = init_server();

  // server_listen(socket, handle_request, 1, (void **)(&root));
  params_t *params_client = pack_params(socket, true, handle_request, &root);

  HANDLE client_thread = create_listen_thread(params_client);

  if (_kbhit()) {
    if (_getch() == 'q')
      goto close;
  }

  WaitForSingleObject(client_thread, INFINITE);

// Label close.
close:
  CloseHandle(client_thread);
  //delete_tree_r(root);
  root = NULL;
  print_tree_r(root);
  free(params_client);

  return 0;
}

void handle_request(SOCKET socket, char *buffer, int size, int request_code,
                    node_t **root) {
  int received_value = atoi(buffer);

  char response_buffer[11];

  switch (request_code) {
  case SIGN_IN:
    printf("Received node: %d\n", received_value);
    insert(root, received_value);
    break;
  case VALUE:
    printf("Received value: %d\n", received_value);
    // TODO: find_linear(&root, value);
    //_itoa_s(received_value, response_buffer, 11, 10);
    // int size = strlen(response_buffer);
    // send((SOCKET)params[argc - 1], response_buffer, &size);
    break;
  case PID:
    printf("RECEIVED PID: %d\n", received_value);
    node_t *node = find(root, received_value);
    send_response(socket, response_buffer, 11, received_value);
    break;
  }
  print_tree_r(*root);
}

void send_response(SOCKET socket, char *buffer, int buffer_len, int response) {
  int size = strlen(buffer);
  _itoa_s(response, buffer, buffer_len, 10);
  send(socket, buffer, &size);
}

HANDLE create_listen_thread(params_t *params) {
  DWORD threadID;
  return CreateThread(NULL, 0, listen_thread, params, 0, &threadID);
}

DWORD WINAPI listen_thread(LPVOID lpParam) {
  params_t params = *(params_t *)lpParam;
  server_listen(params.socket, params.RECV_THREAD, params.request_handler,
                params.root);
  return 0;
}

// Packs params for the thread function.
params_t *pack_params(SOCKET socket, bool RECV_THREAD, HANDLE_R request_handler,
                      node_t **root) {
  params_t *params = (params_t *)malloc(sizeof(params_t));
  params->socket = socket;
  params->RECV_THREAD = RECV_THREAD;
  params->request_handler = handle_request;
  params->root = root;

  return params;
}
