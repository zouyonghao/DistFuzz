#include <arpa/inet.h> //inet_addr
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h> //write
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int server_socket, client_socket, c;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int enable = 0;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    struct linger linger;
    linger.l_onoff = 1;
    linger.l_linger = 0;
    setsockopt(server_socket, SOL_SOCKET, SO_LINGER, (const char *)&linger, sizeof(linger));

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 3);

    // // set nonblocking
    // fcntl(server_socket, F_SETFL, fcntl(server_socket, F_GETFL, 0) | O_NONBLOCK);

    c = sizeof(struct sockaddr_in);
    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&c)) > 0)
    {
        uint8_t client_message[1000];
        clock_t begin = clock();
        int read_length = 0;
        while (read_length <= 0)
            read_length = read(client_socket, client_message, 1000);
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("server read time usage is %f\n", time_spent);
        break;
    }
    close(client_socket);
    close(server_socket);
    return 0;
}
