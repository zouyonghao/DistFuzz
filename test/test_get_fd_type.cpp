#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char const *argv[])
{
    unsigned int val = 0;
    unsigned int val_len = sizeof(val);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int ret = getsockopt(sock, SOL_SOCKET, SO_TYPE, &val, &val_len);

    assert(errno != ENOTSOCK);
    assert(val == SOCK_STREAM);

    FILE *f = fopen(".", "r");
    int fd = fileno(f);
    if ((ret = getsockopt(fd, SOL_SOCKET, SO_TYPE, &val, &val_len)) > 0)
    {
        perror("getsockopt fail");
    }

    assert(errno == ENOTSOCK);

    return 0;
}
