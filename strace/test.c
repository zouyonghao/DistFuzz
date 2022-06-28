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

int
main()
{
	int sz;

	int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) {
		perror("r1");
		exit(1);
	}

	sz = write(fd, "a\n", 2);
	char *bytes = "bbbbbbbbbbbbbbbb\n";
	sz = write(fd, bytes, strlen(bytes));

	int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(9999);

	connect(sock, &server, sizeof(server));

	char *bytes3 = "ffff\n";
	write(sock, bytes3, strlen(bytes3));

	sz = write(sock, "c\n", 2);
	sz = write(sock, "d\n", 2);

	char *bytes2 = "ee\n";
	struct iovec iovs[2] = { { .iov_base = bytes2, .iov_len = strlen(bytes2) },
				 { .iov_base = bytes2, .iov_len = strlen(bytes2) } };
	sz = writev(sock, iovs, 2);

	close(fd);
	close(sock);
}
