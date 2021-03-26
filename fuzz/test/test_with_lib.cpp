#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <vector>

extern void test();

int main(int argc, char const *argv[])
{
    unsigned char tmp = 0;

    unsigned int result = tmp;
    result <<= 8u;

    tmp = 1;
    result |= tmp;
    result <<= 8u;

    tmp = 0;
    result |= tmp;
    result <<= 8u;

    tmp = 0;
    result |= tmp;

    printf("result = %d\n", result);

    std::vector<const char *> test_vector{"echo", "abc"};
    execvp(test_vector[0], (char *const*) &test_vector[0]);

    test();

    return 0;
}
