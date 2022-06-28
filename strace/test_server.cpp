#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/uio.h>
#include <arpa/inet.h>

#include <algorithm>
#include <future>
#include <iostream>
#include <list>

int
main()
{
	int socket_desc, client_sock, c;
	struct sockaddr_in server, client;
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc < 0) {
		std::cout << "Could not create socket" << std::endl;
		return;
	}
	std::cout << "Socket created" << std::endl;

	int enable = 0;

	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(9999);

	// Bind
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
		// print the error message
		perror("bind failed. Error");
		return;
	}
	std::cout << "bind done\n";

	// Listen
	listen(socket_desc, 3);

	// Accept and incoming connection
	std::cout << "Waiting for incoming connections...\n";
	c = sizeof(struct sockaddr_in);
	while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))) {
		int read_size;
		uint8_t client_message[1000];
		while ((read_size = read(client_sock, client_message, 1000)) > 0) {
			for (int i = 0; i < read_size; i++) {
				std::cout << client_message[i];
			}
		}
	}

	if (client_sock < 0) {
		perror("accept failed");
	}

	printf("stoped!\n");

	close(socket_desc);
}