//
// Created by zyh on 2021.10.18.
//

#include "dst_fuzz_constant.h"
#include "utils/dst_share_mem_util.h"
#include <cstdlib>
#include <iostream>
#include <sys/shm.h>

static bool init_done = false;
static uint8_t *is_fuzzing;
static int share_mem_id = 0;

bool init_is_fuzzing()
{
    share_mem_id = shmget(IPC_PRIVATE, sizeof(uint8_t), IPC_CREAT | IPC_EXCL | 0600);
    if (share_mem_id < 0)
    {
        std::cerr << "Fail to get share memory\n";
        exit(-1);
    }

    if (setenv(IS_FUZZING_SHM_ENV_VAR, std::to_string(share_mem_id).c_str(), 1))
    {
        std::cerr << "Fail to set env\n";
        exit(-1);
    }

    is_fuzzing = static_cast<uint8_t *>(shmat(share_mem_id, nullptr, 0));
    if (!is_fuzzing)
    {
        std::cerr << "Fail to locate share memory\n";
        exit(-1);
    }
    *is_fuzzing = 1;
    return true;
}

bool set_is_fuzzing(bool should_fuzzing)
{
    get_is_fuzzing();
    if (should_fuzzing)
    {
        *is_fuzzing = 1;
    }
    else
    {
        *is_fuzzing = 0;
    }
    return true;
}

bool get_is_fuzzing()
{
    if (!init_done)
    {
        char *is_fuzzing_env = getenv(IS_FUZZING_SHM_ENV_VAR);
        if (!is_fuzzing_env)
        {
            std::cerr << "\033[33mYou are running in a non-fuzzing environment\033[0m\n";
            is_fuzzing = static_cast<uint8_t *>(malloc(sizeof(uint8_t)));
            *is_fuzzing = 1;
        }
        else
        {
            std::cerr << "\033[33mYou are running in a fuzzing environment\033[0m\n";
            int shmId = strtol(is_fuzzing_env, nullptr, 0);
            is_fuzzing = static_cast<uint8_t *>(shmat(shmId, nullptr, 0));
        }
        init_done = true;
    }
    return *is_fuzzing == 1;
}

bool remove_is_fuzzing()
{
    if (share_mem_id != 0)
    {
        return shmctl(share_mem_id, IPC_RMID, nullptr) == 0;
    }
    return true;
}

void increase_coverage(uint64_t hash_index)
{
    hash_index = hash_index % FUZZ_COVERAGE_MAP_SIZE;
    static uint8_t *fuzz_coverage_map = nullptr;
    if (fuzz_coverage_map == nullptr)
    {
        char *res_shm_fuzz_coverage_map = getenv(FUZZ_COVERAGE_MAP_ENV_ID);
        if (!res_shm_fuzz_coverage_map)
        {
            fprintf(stderr, "%s", "\033[33m[FUZZ PRINT] Can not Get Environment Variable\033[0m\n");
        }
        else
        {
            fprintf(stderr, "\033[33m[FUZZ PRINT]Get Environment Variable %s\033[0m\n", res_shm_fuzz_coverage_map);
            fuzz_coverage_map = (uint8_t *)shmat((int)atoi(res_shm_fuzz_coverage_map), NULL, 0);
        }
    }
    fuzz_coverage_map[hash_index]++;
}