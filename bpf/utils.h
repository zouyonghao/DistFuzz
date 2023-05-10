#ifndef BPF_FAULT_UTILS_H
#define BPF_FAULT_UTILS_H

static inline int max(int a, int b) { return a > b ? a : b; }

static inline int min(int a, int b) { return a > b ? b : a; }

static inline int get_bit(unsigned char *array, int array_size, unsigned int bit_index)
{
    unsigned int elem_length = (sizeof(unsigned char) * 8);
    unsigned int char_index = bit_index / elem_length;
    if (char_index >= 0 && char_index < array_size)
    {
        unsigned char elem = array[char_index];
        bit_index = bit_index % elem_length;
        return (elem >> bit_index) & 0x1;
    }
    return 0;
}

/**
 * return 0 for equal
 */
static inline int str_equal(const char *cs, const char *ct, int size)
{
    int len = 0;
    unsigned char c1, c2;
    /**
     * NOTE: use & 0xffff to make verifier happy.
     */
    for (len = 0; len < (size & 0xffff); len++)
    {
        c1 = *cs++;
        c2 = *ct++;
        if (c1 != c2)
            return c1 < c2 ? -1 : 1;
        if (!c1)
            break;
    }
    return 0;
}

/**
 * return 0 for containing
 */
static inline int str_contains(const char *str, const char *pattern, int str_size, int pattern_size)
{
    int index = 0;
    int pattern_index = 0;
    unsigned char c1, c2;
    /**
     * NOTE: use & 0xffff to make verifier happy.
     */
    for (index = 0; index < ((str_size - pattern_size) & 0xffff); index++)
    {
        c1 = *(str + index + pattern_index);
        /**
         * NOTE: It seems the str may contains data more than a string,
         * so we need to know where the string ends.
         */
        if (c1 == 0)
        {
            return 1;
        }
        if (str_equal(str + index, pattern, pattern_size) == 0)
        {
            return 0;
        }
    }
    return 1;
}

#endif