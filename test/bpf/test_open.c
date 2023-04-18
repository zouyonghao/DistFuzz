#include <stdio.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd = open("/tmp/secret.txt", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    printf("Opened /tmp/secret.txt successfully\n");
    close(fd);
    return 0;
}