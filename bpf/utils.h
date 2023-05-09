#ifndef BPF_FAULT_UTILS_H
#define BPF_FAULT_UTILS_H

static inline int max(int a, int b) { return a > b ? a : b; }

static inline int min(int a, int b) { return a > b ? b : a; }

static inline int is_current_pid_or_tgid(int pid)
{
    u32 current_pid = bpf_get_current_pid_tgid() >> 32;
    u32 current_tgid = bpf_get_current_pid_tgid();

    if (current_pid != pid && current_tgid != pid)
    {
        return 0;
    }
    return 1;
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