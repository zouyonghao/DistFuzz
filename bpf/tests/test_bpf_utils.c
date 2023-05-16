
#include <stdbool.h>
#include "../utils.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    assert(str_contains("/usr/lib/locale/en.utf8/LC_NUMERIC", ".o", 24, 2) == false);
    assert(str_contains("/usr/lib/locale/en.otf8/LC_NUMERIC", ".o", 24, 2) == true);
    assert(str_contains(".o", ".o", 20, 2) == true);

    printf("sizeof(unsigned char) = %lu\n", sizeof(unsigned char));
    unsigned char test[] = {0b00000010, 0b01, 0b00110010};
    // first char
    assert(get_bit(test, 3, 0) == 0);
    assert(get_bit(test, 3, 1) == 1);
    assert(get_bit(test, 3, 2) == 0);
    assert(get_bit(test, 3, 3) == 0);
    assert(get_bit(test, 3, 4) == 0);
    assert(get_bit(test, 3, 5) == 0);
    assert(get_bit(test, 3, 6) == 0);
    assert(get_bit(test, 3, 7) == 0);

    // second char
    assert(get_bit(test, 3, 8) == 1);
    assert(get_bit(test, 3, 9) == 0);
    assert(get_bit(test, 3, 10) == 0);
    assert(get_bit(test, 3, 11) == 0);
    assert(get_bit(test, 3, 12) == 0);
    assert(get_bit(test, 3, 13) == 0);
    assert(get_bit(test, 3, 14) == 0);
    assert(get_bit(test, 3, 15) == 0);

    // third char
    assert(get_bit(test, 3, 16) == 0);
    assert(get_bit(test, 3, 17) == 1);
    assert(get_bit(test, 3, 18) == 0);
    assert(get_bit(test, 3, 19) == 0);
    assert(get_bit(test, 3, 20) == 1);
    assert(get_bit(test, 3, 21) == 1);
    assert(get_bit(test, 3, 22) == 0);
    assert(get_bit(test, 3, 23) == 0);
    return 0;
}
