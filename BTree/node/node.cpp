// node.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../socket/socket.h"

#include <stdlib.h>

#pragma comment (lib, "../Debug/socket.lib")

/*
	REQUEST codes used by node:
	SIGN_IN = 2
	SING_OUT = 3
*/
enum REQUSET_CODE{SIGN_IN = 2, SIGN_OFF};

int main(int argc, char **argv)
{
	/* Connect to server. */
	SOCKET socket = init_client();

	int size;
	int request_code = 2;

	/* Send request_code. */
	size = 4;
	send(socket, (char *)(&request_code), &size);
	
	int pid = atoi(argv[1]);
	send(socket, (char *)(&pid), &size);


    return 0;
}

