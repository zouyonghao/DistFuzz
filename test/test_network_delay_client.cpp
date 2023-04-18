#include <arpa/inet.h> //inet_addr
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <time.h>
#include <unistd.h> //write

int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in server;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // set nonblocking
    // fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);

    // Bind to a specific network interface (and optionally a specific local port)
    // struct sockaddr_in localaddr;
    // localaddr.sin_family = AF_INET;
    // localaddr.sin_addr.s_addr = inet_addr(argv[3]);
    // localaddr.sin_port = 0; // Any local port will do
    // bind(sock, (struct sockaddr *)&localaddr, sizeof(localaddr));

    int enable;
    // setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    // setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));

    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));

    printf("connecting\n");
    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed.");
        // return -1;
    }
    printf("connected\n");

    const char *s = "abcdefg";

    // struct iovec vec[10];
    // for (int i = 0; i < 10; i++)
    // {
    //     vec[i].iov_base = (void *)s;
    //     vec[i].iov_len = strlen(s) + 1;
    // }

    clock_t begin = clock();
    for (int i = 0; i < 1000000; i++)
    {
        // printf("client writing\n");
        int length = strlen(s) + 1;
        int total_sent = 0;
        int result = 1;
        while (result > 0)
        {
            result = write(sock, &s[total_sent], length - total_sent);
            // result = writev(sock, vec, 10);
            total_sent += result;
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("client write time usage is %f\n", time_spent);
    close(sock);

    // sleep(1000);
    return 0;
}
