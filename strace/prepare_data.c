#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
int
main()
{
	int sz;

	int fd = open("dst_fuzz_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) {
		perror("r1");
		exit(1);
	}

	// delay
	sz = write(fd, "\xff", sizeof(char));
	sz = write(fd, "\x00", sizeof(char));
	sz = write(fd, "\xff", sizeof(char));
	sz = write(fd, "\x00", sizeof(char));

	// fail
	sz = write(fd, "\xff", sizeof(char));
	sz = write(fd, "\x01", sizeof(char));
	sz = write(fd, "\x03", sizeof(char)); // return error code

	// success
	sz = write(fd, "\x00", sizeof(char));

	// lost
	sz = write(fd, "\xff", sizeof(char));
	sz = write(fd, "\x02", sizeof(char));

	// dup
	sz = write(fd, "\xff", sizeof(char));
	sz = write(fd, "\x03", sizeof(char));

	// dup
	sz = write(fd, "\xff", sizeof(char));
	sz = write(fd, "\x03", sizeof(char));

	close(fd);
}
