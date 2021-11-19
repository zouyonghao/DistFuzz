#ifndef DST_RANDOM_HEADER
#define DST_RANDOM_HEADER

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 *
 * dst_random is not a random generator!
 * It helps read from a file and output its content as a user speficied
 * like following methods.
 * @see __dst_get_random_char
 * @see __dst_get_random_uint16_t
 * @see __dst_get_random_uint32_t
 *
 * The file is specified by environment variable __DST_ENV_RANDOM_FILE__
 * in the default implementation dst_default_random.c
 * e.g. __DST_ENV_RANDOM_FILE__=random.txt ./a.out
 *
 * @author Yong-Hao Zou
 * @date 2021/03/17
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif

    extern uint8_t __dst_get_random_uint8_t();

    extern uint16_t __dst_get_random_uint16_t();

    extern uint32_t __dst_get_random_uint32_t();

    extern void __dst_reinit_random(const char *file_name);

    extern char *__dst_get_random_string(uint32_t length);

#ifdef __cplusplus
}
#endif

#endif // DST_RANDOM_HEADER