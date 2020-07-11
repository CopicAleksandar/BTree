#include "stdafx.h"

#include "../libbtree/btree.h"
#include "socket.h"

#include <conio.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//#define LARGE_BUFFER
#define REC_THREAD

#define DEFAULT_PORT 27015
#define DEFAULT_PORT_PCSTR "27015"
#define SERVER_SLEEP_TIME 50
//#define SERVER_IP "127.0.0.1"
#define SERVER_IP "192.168.1.6"

#ifdef LARGE_BUFFER
#define BUFFER_LEN 100000001
#else
#define BUFFER_LEN 512
#endif

bool init_windows_sockets() {
  WSADATA wsaData;

  // Initialize windows sockets library for this process
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    printf("WSAStartup failed with error: %d\n", WSAGetLastError());
    return false;
  }

  return true;
}

SOCKET init_server() {
  // Socket used for listening for new clients
  SOCKET listenSocket = INVALID_SOCKET;

  // Socket used for communication with client
  SOCKET acceptedSocket = INVALID_SOCKET;

  // variable used to store function return value
  int iResult;

  if (init_windows_sockets() == false) {
    // we won't log anything since it will be logged
    // by InitializeWindowsSockets() function
    return 1;
  }

  // Prepare address information structures
  addrinfo *resultingAddress = NULL;
  addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;       // IPv4 address
  hints.ai_socktype = SOCK_STREAM; // FD_SET set;

  timeval timeVal;
  timeVal.tv_sec = 0;
  timeVal.tv_usec = 0;             // Provide reliable data streaming
  hints.ai_protocol = IPPROTO_TCP; // Use TCP protocol
  hints.ai_flags = AI_PASSIVE;     //

  // Resolve the server address and port
  iResult = getaddrinfo(NULL, DEFAULT_PORT_PCSTR, &hints, &resultingAddress);
  if (iResult != 0) {
    printf("getaddrinfo failed with error: %d\n", iResult);
    WSACleanup();
    return 1;
  }

  // Create a SOCKET for connecting to server
  listenSocket = socket(AF_INET,      // IPv4 address famly
                        SOCK_STREAM,  // stream socket
                        IPPROTO_TCP); // TCP

  if (listenSocket == INVALID_SOCKET) {
    printf("socket failed with error: %ld\n", WSAGetLastError());
    freeaddrinfo(resultingAddress);
    WSACleanup();
    return 1;
  }

  // Setup the TCP listening socket - bind port number and local address
  // to socket
  iResult = bind(listenSocket, resultingAddress->ai_addr,
                 (int)resultingAddress->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
    printf("bind failed with error: %d\n", WSAGetLastError());
    freeaddrinfo(resultingAddress);
    closesocket(listenSocket);
    WSACleanup();
    return 1;
  }

  unsigned long int iMode = 1;
  iResult = ioctlsocket(listenSocket, FIONBIO, &iMode);

  freeaddrinfo(resultingAddress);

  return listenSocket;
}

// void server_listen(SOCKET socket, WAIT_MODE mode,
//                 void (*f)(char *, int, int, int, void **), int argc,
//                   void **argv, ...) {

void server_listen(SOCKET socket, bool RECV_THREAD, HANDLE_R request_handler,
                   node_t **root) {
  // Buffer used for storing incoming data
#ifdef LARGE_BUFFER // put large buffer on heap
  char *recvbuf = (char *)malloc(LARGE_BUFFER_LEN);
#else
  char recvbuf[BUFFER_LEN];
#endif

  // auto root_listen = argv;
  FD_SET readfds;
  timeval timeVal;

  // Set listenSocket in listening mode
  int iResult = listen(socket, SOMAXCONN);
  if (iResult == SOCKET_ERROR) {
    printf("listen failed with error: %d\n", WSAGetLastError());
    closesocket(socket);
    WSACleanup();
    return;
  }

  int size, handle_cnt = 0;
  printf("Server started listening for clients...\nTo quit press 'q'\n");

  HANDLE handles[100];
  params_t *lpParams = NULL;

  while (1) {
    // if 'q' is pressed, stop listening. Cleanup.
    if (_kbhit()) {
      if (_getch() == 'q')
        break;
    }

    // Wait for clients and accept client connections.
    // Returning value is acceptedSocket used for further
    // Client<->Server communication. This version of
    // server will handle only one client.
    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);

    timeVal.tv_sec = 0;
    timeVal.tv_usec = 0;
    iResult = select(0, &readfds, NULL, NULL, &timeVal);

    if (iResult == SOCKET_ERROR) {
      fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
      continue;
    }

    // now, lets check if there are any sockets ready
    if (iResult == 0) {
      // there are no ready sockets, sleep for a while and check again
      Sleep(SERVER_SLEEP_TIME);
      continue;
    }

    SOCKET acceptedSocket = accept(socket, NULL, NULL);

    if (acceptedSocket == INVALID_SOCKET) {
      printf("accept failed with error: %d\n", WSAGetLastError());
      closesocket(socket);
      WSACleanup();
      return;
    }

    unsigned long int iMode = 1;
    iResult = ioctlsocket(acceptedSocket, FIONBIO, &iMode);

    // params[i + 1] = (void **)socket;

    if (RECV_THREAD) {
      // lpParams can be freed since the thread saved it on its stack.
      if (lpParams)
        free(lpParams);
      lpParams = (params_t *)malloc(sizeof(params_t));
      lpParams->socket = acceptedSocket;
      lpParams->request_handler = request_handler;
      lpParams->root = root;

      //++handle_cnt;
      handles[handle_cnt++] = create_thread(lpParams);
    } else {
      receive(acceptedSocket, recvbuf);
      int request_code = *(int *)(recvbuf);
      int size;

      size = receive(acceptedSocket, recvbuf);
      request_handler(acceptedSocket, recvbuf, size, request_code, root);

      char *msg = "this is an answer";
      size = strlen(msg);
      send(acceptedSocket, msg, &size);
    }
  }

  if (handle_cnt > 0) {
    CloseHandle(handles[0]);
    // This free only frees last allocation of lpParams -> leaks memory.
    free(lpParams);
  }
}

HANDLE create_thread(params_t *params) {
  DWORD threadID;
  return CreateThread(NULL, 0, thread, params, 0, &threadID);
}

DWORD WINAPI thread(LPVOID lpParam) {
  printf("From thread\n");
  char buffer[BUFFER_LEN];
  int request_code, size;

  params_t params = *(params_t *)lpParam;

  // Socket is needed in request handler, to send answer.
  receive(params.socket, buffer);
  request_code = *(int *)(buffer);

  while (1) {
    size = receive(params.socket, buffer);
	// Client unreachable.
	if (size == -1)
		return 0;

    HANDLE_R request_handler = params.request_handler;
    request_handler(params.socket, buffer, size, request_code, params.root);

    char *msg = "this is an answer";
    size = strlen(msg);
    send(params.socket, msg, &size);
  }

  return 0;
}

int receive_data(SOCKET socket, char *buffer, int size) {
  int iResult = 0, offset = 0;

  FD_SET readfds;
  timeval timeVal;
  timeVal.tv_sec = 0;
  timeVal.tv_usec = 0;

  while ((size -= iResult) > 0) {
    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);

    iResult = select(0, &readfds, NULL, NULL, &timeVal);

    if (iResult == SOCKET_ERROR) {
      // TODO: node deletion logic and client signoff
      //fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
      fprintf(stderr, "Client signed off: %ld", WSAGetLastError());
      iResult = 0;
	  closesocket(socket);
      return -1;
    }

    // now, lets check if there are any sockets ready
    if (iResult == 0) {
      // there are no ready sockets, sleep for a while and check again
      Sleep(SERVER_SLEEP_TIME);
      continue;
    }
    iResult = 0;
    iResult = recv(socket, buffer + offset, size, 0);

    if (iResult > 0)
      printf("Received: %d\n", iResult);
    else if (iResult == 0) {
      printf("Connection with client closed.\n");
      closesocket(socket);
    } else {
      printf("recv failed with error: %d\n", WSAGetLastError());
      closesocket(socket);
    }

    offset += iResult;
  }

  return offset;
}

SOCKET init_client() {
  // socket used to communicate with server
  SOCKET connectSocket = INVALID_SOCKET;
  // variable used to store function return value
  // int iResult;
  // message to send
  char *messageToSend = "this is a test";

  if (init_windows_sockets() == false) {
    // we won't log anything since it will be logged
    // by InitializeWindowsSockets() function
    return 1;
  }

  // create a socket
  connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (connectSocket == INVALID_SOCKET) {
    printf("socket failed with error: %ld\n", WSAGetLastError());
    WSACleanup();
    return 1;
  }

  // create and initialize address structure
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
  serverAddress.sin_port = htons(DEFAULT_PORT);

  FD_SET writefds;
  timeval timeVal;
  timeVal.tv_sec = 0;
  timeVal.tv_usec = 0;

  int iResult;
  while (1) {

    /*
// connect to server specified in serverAddress and socket connectSocket
FD_ZERO(&writefds);
FD_SET(connectSocket, &writefds);
iResult = select(0, NULL, &writefds, NULL, &timeVal);
if (iResult == SOCKET_ERROR) {
fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
iResult = 0;
continue;
}

// now, lets check if there are any sockets ready
if (iResult == 0) {
// there are no ready sockets, sleep for a while and check again
Sleep(SERVER_SLEEP_TIME);
continue;
}
  */

    if (connect(connectSocket, (SOCKADDR *)&serverAddress,
                sizeof(serverAddress)) == SOCKET_ERROR) {
      printf("Unable to connect to server.\nError: %d\n", WSAGetLastError());
      closesocket(connectSocket);
      WSACleanup();
    }
    unsigned long int iMode = 1;
    iResult = ioctlsocket(connectSocket, FIONBIO, &iMode);
    return connectSocket;
  }

  return connectSocket;
}

int send(SOCKET socket, char *buffer, int *size) {
  int ret_val = send_data(socket, (char *)size, 4);
  if (ret_val == -1)
	  return ret_val;
  return send_data(socket, buffer, *size);
}

int receive(SOCKET socket, char *buffer) {
  int size = receive_data(socket, buffer, 4);

  if (size != 4)
    return -1;

  size = *(int *)buffer;
  return receive_data(socket, buffer, size);
}

int send_data(SOCKET socket, char *buffer, int size) {
  FD_SET writefds;
  timeval timeVal;
  timeVal.tv_sec = 0;
  timeVal.tv_usec = 0;

  int iResult = 0, offset = 0;

  while ((size -= iResult) > 0) {
    FD_ZERO(&writefds);
    FD_SET(socket, &writefds);
    iResult = select(0, NULL, &writefds, NULL, &timeVal);
    if (iResult == SOCKET_ERROR) {
      //fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
      fprintf(stderr, "client signed off: %ld", WSAGetLastError());
      iResult = 0;
	  closesocket(socket);
      return -1;
    }

    // now, lets check if there are any sockets ready
    if (iResult == 0) {
      // there are no ready sockets, sleep for a while and check again
      Sleep(SERVER_SLEEP_TIME);
      continue;
    }

    iResult = 0;
    iResult = send(socket, buffer + offset, size - offset, 0);
    printf("sent %d bytes\n", iResult);

    if (iResult == SOCKET_ERROR) {
      printf("send failed with error: %d\n", WSAGetLastError());
      closesocket(socket);
      WSACleanup();
      return 1;
    }

    offset += iResult;
  }
  return offset;
}

void cleanup(SOCKET socket) {
  closesocket(socket);
  WSACleanup();
}
