#include <assert.h>
#include <constants.h>
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

#define SHM_ENV_VAR "AFLCplusplus_SHM_ID"
#define BRANCH_TRACE_SHM_ENV_VAR "AFLCplusplus_BRANCH_TRACE_SHM_ID"
#define CONCURRENT_FUNCTION_SHM_ENV_VAR "AFLCplusplus_CONCURRENT_FUNCTION_SHMD_ID"
#define CONCURRENT_MUTEX_SHM_ENV_VAR "AFLCplusplus_CONCURRENT_MUTEX_SHMD_ID"
#define TICK_NUM_SHM_ENV_VAR "AFLCplusplus_TICK_NUM_SHMD_ID"
#define RAW_THREAD_ID_VEC_SHM_ENV_VAR "AFLCplusplus_RAW_THREAD_ID_VEC_SHMD_ID"
#define THREAD_ID_VEC_SHM_ENV_VAR "AFLCplusplus_THREAD_ID_VEC_SHMD_ID"
#define THREAD_COUNT_SHM_ENV_VAR "AFLCplusplus_THREAD_COUNT_SHMD_ID"

#define FORSRV_FD 198

#define INIT_STACK_SIZE 1000

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
static u8 *branchTraceBit;
static u64 *concurrentFunctionCountVar;
static u64 *tickNum;
static pthread_mutex_t *multiProcessMutex;
static pthread_mutex_t singleProcessMutex = PTHREAD_MUTEX_INITIALIZER;
static u32 threadTickNum = 0;
static u32 node_id;

static u32 *node_id_vec;
static u32 node_count = 3;

static u64 raw_thread_id_vec[1000] = {0u}; // the thread id from system
// static u32 thread_id_vec[1000];           // the thread id allocated by fuzzer
static u32 thread_count = 0;

static u8 shmEnable = 0;

static u8 init_done = 0;

__thread u32 prevLoc = 0;
__thread u64 *call_stack;
__thread u32 stack_top = 0;
__thread u32 stack_size = 0;

__thread u8 is_new_thread = 1;

u32 thread_exists(pthread_t thread_id)
{
    char path[100];
    FILE *fp;
    sprintf(path, "/proc/%d/task/%lu/comm", getpid(), thread_id);
    fp = fopen(path, "r");
    if (fp != NULL)
    {
        fclose(fp);
        return 1;
    }
    fprintf(stderr, "thread %ld dose not exist now.\n", thread_id);
    return 0;
}

void ProcessTick(u32 curLoc)
{
    if (init_done == 0)
        return;

    // printf("111\n");
    if (shmEnable == 0)
        return;

    u64 raw_thread_id = syscall(__NR_gettid); // pid would change during each execution

    pthread_mutex_lock(multiProcessMutex);
    (*tickNum)++;
    pthread_mutex_unlock(multiProcessMutex);
    // TODO: what should we do if a node dies?
    // fprintf(stderr, "we are executing basic block %d, node_id is %d, node_count is %d\n", curLoc, node_id,
    // node_count);
    u64 wait_count = 0;
    while ((*tickNum) % (node_count) != node_id) // wait until we can continue
    {
        wait_count++;
        if (wait_count % 10000000u == 0)
        {
            // fprintf(stderr, "we have wait for %ld cycles\n", wait_count);
            // fprintf(stderr, "Threads: ");
            // for (int i = 0; i < thread_count; i++)
            // {
            //     fprintf(stderr, "%u ", raw_thread_id_vec[i]);
            // }
            // fprintf(stderr, "thread %lu get stucked at %d\n", raw_thread_id, curLoc);
        }
    }

    // fprintf(stderr, "thread %lu is executing basic block %d\n", raw_thread_id, curLoc);
    if (is_new_thread && !thread_exists(raw_thread_id))
    {
        fprintf(stderr, "we have a new thread %lu but thread_exists does not think so.\n", raw_thread_id);
    }
    // fprintf(stderr, "thread %lu try to get lock\n", raw_thread_id);
    pthread_mutex_lock(&singleProcessMutex);
    for (u32 i = 0; i < thread_count;)
    {
        if (!thread_exists(raw_thread_id_vec[i]))
        {
            thread_count--;
            for (u32 j = i; j < thread_count; j++)
            {
                raw_thread_id_vec[j] = raw_thread_id_vec[j + 1];
            }
            fprintf(stderr, "thread %lu dose not exist now, current thread_cout is : %u\n", raw_thread_id_vec[i],
                    thread_count);
        }
        else
        {
            i++;
        }
        // fprintf(stderr, "i = %d.\n", i);
    }

    if (is_new_thread) // TODO: instrument pthread_create call here!
    {
        raw_thread_id_vec[thread_count] = raw_thread_id;
        thread_count++;
        is_new_thread = 0;
        fprintf(stderr, "we have a new thread %lu.\n", raw_thread_id);
    }

    threadTickNum++;
    pthread_mutex_unlock(&singleProcessMutex);
    // fprintf(stderr, "thread %lu unlocked\n", raw_thread_id);
    // TODO: what should we do if a thread dies?
    wait_count = 0;
    while (raw_thread_id_vec[threadTickNum % thread_count] != raw_thread_id)
    {
        // wait until we can continue
        wait_count++;
        if (wait_count % 10000000u == 0)
        {
            // fprintf(stderr, "we have wait for %ld cycles\n", wait_count);
            // fprintf(stderr, "Threads: ");
            // for (int i = 0; i < thread_count; i++)
            // {
            //     fprintf(stderr, "%u ", raw_thread_id_vec[i]);
            // }
            // fprintf(stderr, "thread %lu get stucked at %d\n", raw_thread_id, curLoc);
        }
        // usleep(0);
    }
    // fprintf(stderr, "we are executing basic block %d, thread_count is %d\n", curLoc, thread_count);
}

void FuncSequenceRecord(u32 curLoc)
{
    curLoc += node_id;
    if (init_done == 0)
        return;
    // printf("111\n");
    if (shmEnable == 0)
        return;
    // TODO: remove the lock by async the operations
    pthread_mutex_lock(multiProcessMutex);
    // // NOTE: This is not the sequence coverage, this is the coverage of
    // // jumping between different node function
    // u64 index = ((*concurrentFunctionCountVar) ^ curLoc) % MAP_SIZE;
    // if (branchTraceBit[index] < 255)
    // {
    //     branchTraceBit[index]++;
    // }
    // (*concurrentFunctionCountVar) = curLoc >> 1;
    //

    u64 index = ((*concurrentFunctionCountVar) ^ curLoc) % MAP_SIZE;
    if (branchTraceBit[index] < 255)
    {
        branchTraceBit[index]++;
    }
    (*concurrentFunctionCountVar) = ((*concurrentFunctionCountVar) ^ curLoc) >> 1;
    pthread_mutex_unlock(multiProcessMutex);
}

void FuncEnterRecord(u32 curLoc)
{
    if (init_done == 0)
        return;
    // printf("111\n");
    if (shmEnable == 0)
        return;
    pthread_mutex_lock(multiProcessMutex);
    if (stack_size == 0)
    {
        call_stack = (u64 *)calloc(INIT_STACK_SIZE, sizeof(u64));
        if (call_stack == NULL)
        {
            fprintf(stderr, "malloc failed\n");
        }
        stack_size = INIT_STACK_SIZE;
    }
    if (stack_top >= stack_size)
    {
        call_stack = (u64 *)realloc(call_stack, sizeof(u64) * stack_size * 2);
        if (call_stack == NULL)
        {
            fprintf(stderr, "malloc failed\n");
        }
        stack_size *= 2;
    }
    if (stack_top > 0)
    {
        (*concurrentFunctionCountVar) -= call_stack[stack_top - 1];
    }
    (*concurrentFunctionCountVar) += curLoc;
    call_stack[stack_top++] = curLoc;
    pthread_mutex_unlock(multiProcessMutex);
}

void FuncExitRecord(u32 curLoc)
{
    if (init_done == 0)
        return;
    // printf("111\n");
    if (shmEnable == 0)
        return;
    if (call_stack == NULL)
    {
        fprintf(stderr, "call_stack is NULL!\n");
    }

    pthread_mutex_lock(multiProcessMutex);
    if (stack_top > 0)
    {
        while (stack_top > 0 && curLoc != call_stack[stack_top - 1])
        {
            // TODO: why this happen?
            fprintf(stderr, "curLoc != call_stack[stack_top - 1], curLoc is %d, stack_top is %ld\n", curLoc,
                    call_stack[stack_top - 1]);
            (*concurrentFunctionCountVar) -= call_stack[stack_top - 1];
            stack_top--;
        }
        if (stack_top > 0)
        {
            u64 index = (*concurrentFunctionCountVar) % MAP_SIZE;
            if (branchTraceBit[index] < 255)
            {
                branchTraceBit[index]++;
            }
            (*concurrentFunctionCountVar) -= curLoc;
            stack_top--;
            if (stack_top > 0)
                (*concurrentFunctionCountVar) += call_stack[stack_top - 1];
        }
        else
        {
            fprintf(stderr, "A thread called func exit but do not match the func entry! %d\n", curLoc);
        }
    }
    else
    {
        fprintf(stderr, "A thread called func exit without enter a func! %d\n", curLoc);
    }
    pthread_mutex_unlock(multiProcessMutex);
}

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
    char *res_shmBranch = getenv(BRANCH_TRACE_SHM_ENV_VAR);
    char *res_shmConcurrent = getenv(CONCURRENT_FUNCTION_SHM_ENV_VAR);
    char *res_shmMutex = getenv(CONCURRENT_MUTEX_SHM_ENV_VAR);
    char *res_tickNum = getenv(TICK_NUM_SHM_ENV_VAR);
    // char *res_rawThreadIdVec = getenv(RAW_THREAD_ID_VEC_SHM_ENV_VAR);
    // char *res_threadIdVec = getenv(THREAD_ID_VEC_SHM_ENV_VAR);
    // char *res_threadCount = getenv(THREAD_COUNT_SHM_ENV_VAR);

    if (!res_shmCov || !res_shmBranch || !res_shmConcurrent || !res_shmMutex)
    {
        printf("\033[33m[FUZZ PRINT] Can not Get Environment Variable\033[0m\n");
        printf("\033[33m[FUZZ PRINT] res_shmCov = %s\033[0m\n", res_shmCov);
        printf("\033[33m[FUZZ PRINT] Enter Normal Pattern\033[0m\n");

        trace = (u8 *)malloc(MAP_SIZE * sizeof(u8));
        branchTraceBit = (u8 *)malloc(MAP_SIZE * sizeof(u8));
        concurrentFunctionCountVar = (u64 *)malloc(sizeof(u64));
        multiProcessMutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(multiProcessMutex, NULL);
    }
    else
    {
        s32 shmId = atoi(res_shmCov);
        trace = (u8 *)shmat((int)shmId, NULL, 0);

        shmId = atoi(res_shmBranch);
        branchTraceBit = (u8 *)shmat((int)shmId, NULL, 0);

        shmId = atoi(res_shmConcurrent);
        concurrentFunctionCountVar = (u64 *)shmat((int)shmId, NULL, 0);
        *concurrentFunctionCountVar = 0;

        shmId = atoi(res_shmMutex);
        multiProcessMutex = (pthread_mutex_t *)shmat((int)shmId, NULL, 0);

        shmId = atoi(res_tickNum);
        tickNum = (u64 *)shmat((int)shmId, NULL, 0);
        *tickNum = 0;

        // shmId = atoi(res_rawThreadIdVec);
        // raw_thread_id_vec = (u32 *)shmat((int)shmId, NULL, 0);
        // node_id_vec = (u32 *)shmat((int)shmId, NULL, 0);

        // shmId = atoi(res_threadIdVec);
        // thread_id_vec = (u32 *)shmat((int)shmId, NULL, 0);

        // shmId = atoi(res_threadCount);
        // thread_count = (u32 *)shmat((int)shmId, NULL, 0);
        // node_count = (u32 *)shmat((int)shmId, NULL, 0);

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
        pthread_mutex_lock(multiProcessMutex);
        node_id = atoi(getenv("NODE_ID"));
        fprintf(stderr, "node_id is : %u\n", node_id);
        // node_id_vec[*node_count] = node_id;
        // (*node_count)++;
        pthread_mutex_unlock(multiProcessMutex);
        init_done = 1;
    }
    printf("init_in_main success!\n");
}
