#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAP_SIZE_POW2 23 // concurrency coverage is so large
#define MAP_SIZE (1 << MAP_SIZE_POW2)

#define SHM_ENV_VAR "AFLCplusplus_SHM_ID"

#define FORSRV_FD 198

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

// static map<thread::id, threadInfo> threads;
static u8 *trace; // share memory

static u8 shmEnable = 0;

static u8 init_done = 0;

__thread u32 prevLoc = 0;

void CoverageRecord(u32 curLoc)
{
    if (init_done == 0)
        return;
    // printf("111\n");
    if (shmEnable == 0)
        return;
    u32 index = (prevLoc ^ curLoc) % MAP_SIZE;
    // if prevloc and curloc are same in different thread, it will cause data
    // race but if lock for it, the overhead is too big
    if (trace[index] < 255)
    {
        trace[index]++;
    }

    prevLoc = curLoc >> 1;
}

void ShmDeclare(void)
{
    printf("\033[33m[FUZZ PRINT] shm declare\033[0m\n");
    char *res_shmCov = getenv(SHM_ENV_VAR);

    if (!res_shmCov
        // || !res_shmEList || !res_shmETrace
    )
    // || !res_shm_cm || !res_shm_am || !res_shm_tm
    // )
    {
        printf(
            "\033[33m[FUZZ PRINT] Can not Get Environment Variable\033[0m\n");
        printf("\033[33m[FUZZ PRINT] res_shmCov = %s\033[0m\n", res_shmCov);
        printf("\033[33m[FUZZ PRINT] Enter Normal Pattern\033[0m\n");

        trace = (u8 *)malloc(MAP_SIZE * sizeof(u8));
    }
    else
    {
        s32 shmId = atoi(res_shmCov);
        trace = (u8 *)shmat((int)shmId, NULL, 0);
        shmEnable = 1;
        printf("shmEnabled!\n");
    }
}

void ShmDetach(void)
{
    // do nothing
}

void start_forkserver(void)
{
    static u8 tmp[4];
    s32 child_pid;

    u8 child_stopped = 0;

    // tell fuzzer it is ok. If no parent, it is not in fuzzing mode,
    // just return
    if (write(FORSRV_FD + 1, tmp, 4) != 4)
        return;

    while (1)
    {
        u32 was_killed;
        int status;

        if (read(FORSRV_FD, &was_killed, 4) != 4)
            _exit(1);

        if (child_stopped && was_killed)
        {
            child_stopped = 0;
            if (waitpid(child_pid, &status, 0) < 0)
                _exit(1);
        }

        if (!child_stopped)
        {
            child_pid = fork();
            if (child_pid < 0)
                _exit(1);

            // In child process: close fds, resume execution
            if (!child_pid)
            {
                close(FORSRV_FD);
                close(FORSRV_FD + 1);
                return;
            }
        }
        else
        {
            // Special handling for persistent mode:
            // If child is alive but currently stopped, simply restart it with
            // SIGCONT
            kill(child_pid, SIGCONT);
            child_stopped = 0;
        }

        // In parent process: write PID to pipe, then wait for child
        if (write(FORSRV_FD + 1, &child_pid, 4) != 4)
            _exit(1);

        if (waitpid(child_pid, &status, 0) < 0)
            _exit(1);

        // Special handling for persistent mode:
        // In this case, we want to wake it up without forking again
        if (WIFSTOPPED(status))
            child_stopped = 1;

        // Reply wait status to pipe
        if (write(FORSRV_FD + 1, &status, 4) != 4)
            _exit(1);
    }
}

void __attribute__((constructor)) __init_share_memory__(void)
{
    if (!init_done)
    {
        ShmDeclare();
        init_done = 1;
    }
    printf("init_in_main success!\n");
}
