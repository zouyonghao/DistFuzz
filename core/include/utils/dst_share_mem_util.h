//
// Created by zyh on 2021.10.18.
//

#ifndef DISTRIBUTED_SYSTEM_TEST_SHARE_MEM_UTIL_H
#define DISTRIBUTED_SYSTEM_TEST_SHARE_MEM_UTIL_H

#include <stdbool.h>
#include <stdint.h>

/** For dst controlling whether it is still fuzzing the apps. */
#define IS_FUZZING_SHM_ENV_VAR "IS_FUZZING_NUM_SHMD_ID"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Set up is_fuzzing shared variable.
     * This variable is used to indicate whether we are still in the fuzzing progress.
     */
    extern bool init_is_fuzzing();

    extern bool set_is_fuzzing(bool should_fuzzing);

    extern bool get_is_fuzzing();

    extern bool remove_is_fuzzing();

    // extern bool set_is_interesting_seed(bool is_interesting);

    extern void increase_coverage(uint64_t);

#ifdef __cplusplus
};
#endif

#endif // DISTRIBUTED_SYSTEM_TEST_SHARE_MEM_UTIL_H
