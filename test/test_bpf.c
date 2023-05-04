#include "../bpf/utils.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    printf("%d\n", str_contains("/usr/lib/locale/en.utf8/LC_NUMERIC", ".o", 24, 2));
    printf("%d\n", str_contains("/usr/lib/locale/en.otf8/LC_NUMERIC", ".o", 24, 2));
    return 0;
}
