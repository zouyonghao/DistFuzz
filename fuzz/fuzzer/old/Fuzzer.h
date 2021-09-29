#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <pthread.h>

#include "constants.h"

using namespace std;

#define CONTROL_MATRIX_SIZE (1 << 10)
#define ADJACENCY_MATRIX_SIZE (1 << 16)
#define TRAVEL_MATRIX_SIZE (1 << 24)

// can handle 1k * 8 error sites at most
#define MAX_LIST_SIZE_POW2 13
#define MAX_LIST_SIZE (1 << MAX_LIST_SIZE_POW2)

#define SKIP_TO_NEW_PROB 99
#define SKIP_NFAV_OLD_PROB 95
#define SKIP_NFAV_NEW_PROB 75

#define ARITH_MAX 35

#define FUZZING_NORMAL 0
#define FUZZING_SKIP 1
#define FUZZING_ERROR 2

// #define SEEDDIR "./seeddir"
#define HASH_CONST 0xa5b35705

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#define FAULT_NONE 0
#define FAULT_TMOUT 1
#define FAULT_CRASH 2
#define FAULT_ERROR 3
#define FAULT_NOINST 4
#define FAULT_NOBITS 5

// the configure for vim using asan
#define EXEC_TIMEOUT 20000000 // measured by ms
// #define HANG_TIMEOUT	1000 	//measured by ms

struct seed
{
    bool wasFuzzed;
    bool favored;

    string elistFileName;
    string control_matrix_file;
    string fileName;
    u64 fileLen;
    u64 execTime; // measured by us

    bitset<MAP_SIZE> *trace;
    u32 topCount;

    bitset<MAP_SIZE> *concurrent_trace;
    u32 concurrent_top_count;

    u32 cksum;

    bool hasHang;
    bool hasCrash;
    bool trimDone;

    seed()
    {
        wasFuzzed = false;
        favored = false;
        execTime = 0;

        trace = NULL;
        topCount = 0;

        concurrent_trace = NULL;
        concurrent_top_count = 0;

        cksum = 0;
        trimDone = false;
    }
};

typedef list<seed> seed_container;

typedef struct matrix_element
{
    unsigned long long hash_1;
    unsigned long long hash_2;
} matrix_element;

#ifndef __MAIN__
#define __MAIN__
extern seed_container queue;
extern seed_container::iterator queueCur;
extern seed_container::iterator queueTop[MAP_SIZE];
extern seed_container::iterator concurrent_queue_top[MAP_SIZE];
extern u64 queueCycle;
extern bool pendFavor;

extern u8 *globalTraceBit;
extern u8 *virginMap;

extern u8 *globalStateBit;

extern u8 *variableStateChangeCount;
extern u8 *branchTraceBit;
extern u8 *virginMapForBranchTraceBit;

extern u8 *concurrentFunctionCountVar;
extern u8 *tickNum;
extern pthread_mutex_t *concurrentFunctionMutex;

extern u32 *raw_thread_id_vec;
extern u32 *thread_id_vec;
extern u32 *thread_count;

extern bool isBranchCoverage;

extern bitset<MAP_SIZE> timeoutTrace;
extern bitset<MAP_SIZE> crashTrace;
extern u32 unique_crash_count;
extern u32 unique_hang_count;
extern u32 total_crash_count;
extern u32 total_timeout_count;

extern u64 exec_count;
extern u64 total_input;
extern u64 usefule_input;
extern u64 total_error_seq;
extern u64 useful_error_seq;

extern string testfile;
extern string target_path;
extern string inputDir;
extern string outputDir;
extern s32 dev_null_fd;

extern string sync_id;
extern string sync_dir;
extern string sync_state;
extern bool skip_deterministic;
extern int sync_cov;
extern u8 *sync_cov_ptr;
extern int sync_cov_length; // can change
extern string sync_file_name;
extern string file_lock;
extern int master_sync_cycle;
extern int slave_sync_cycle;

extern u8 *sync_branch_cov_ptr;
extern string sync_branch_cov_file_name;

// extern u8 *errorMap;
extern u8 *errorList;
extern u8 *errorTraced;

#ifndef NO_CONCURRENCY_FUZZ
extern matrix_element *control_matrix;
extern matrix_element *adjacency_matrix;
extern matrix_element *travel_matrix;
extern set<u64> total_travel;
#endif
// extern u32 *delayList;
// extern u8 *delayTraced;
// extern u8 *concurrencyMap;
// extern u8 *virginConcurrencyMap;
// extern bitset<MAP_SIZE> concurrency_timeout_trace;
// extern bitset<MAP_SIZE> concurrency_crash_trace;

extern u64 timeBegin;

// extern bitset<MAP_SIZE> ex_timeout_trace;
extern u32 unique_ex_hang_count;
extern u32 total_ex_timeout_count;

extern u32 unique_ex_crash_count;

extern u32 no_forkserver;
extern s32 forkserver_pid;

extern u64 total_cal_us;
extern u64 total_cal_cycles;
extern u32 havoc_div;

extern u8 timeout_given;
extern u32 exec_tmout;
extern u32 hang_tmout;

extern set<u64> unreachable_control_matrix;
extern int concurrency_test_time;
extern int skip_since_travel;
extern int skip_since_unreachable;
#endif // end __MAIN__

// in Main.cpp

// in FuzzOne.cpp
u8 FuzzMain(seed_container::iterator qCur, char **argv);
void print_result(string stage);
u32 ShowCoverage(u8 *virgin_map);

// in TestcasesProcess.cpp
void ReadTestcases(string dirOfCases);
void AddToQueue(const char *fname, u64 fsize, const char *elist_fname = NULL,
                const char *cm_file = NULL);

// in ShmMapping.cpp
void ShmSetup(void);
void ShmRemove(void);
u8 *multi_proc_shm_get(string shm_file);
u8 *multi_proc_shm_create(string shm_file, int mem_size);

// in QueueCulling.cpp
void MinimizeBit(bitset<MAP_SIZE> *trace, u8 *globalTraceBit);
// void UpdateQueueTop(seed_container::iterator s, u8 *globalTraceBit);
void UpdateQueueTop(seed_container::iterator s, u8 *globalTraceBit);
void CullQueue(void);

// in TestRunning.cpp
u32 hash32(const void *key, u32 len, u32 seed);
void DryRunning(char **argv);
void ShowQueue(seed_container::iterator q);
u64 GetCurTimeUs(void);
u8 CalibrateCase(char **argv, seed_container::iterator q, u8 *mem);
u8 ExecuteCase(string target_path, char **argv, u32 timeout);
void WriteToTestcase(void *mem, u32 len, string testfile);
void SetupSignalHandlers(void);
u8 FuzzUnit(char **argv, u8 *filemap, u64 len, u8 *error_list = NULL,
            matrix_element *delay_list = NULL);
u8 AddInterestingSeed(char **argv, u8 *mem, u32 len, u8 run_status);
void ReadFile2Mem(string filename, u8 *mem, u32 len);
void WriteMem2File(string filename, const u8 *const mem, u32 len);
void init_count_class(void);

// in Trim.cpp
u8 TrimCase(char **argv, seed_container::iterator qCur, u8 *inBuf);

// in Sychronize.cpp
void SychronizeFuzzer(char **argv, string syncdir, string syncid);
void fix_up_sync(string sync_id, string &syncDir, string &outputDir);
void sync_coverage(u8 *local_coverage, u8 *global_coverage, int length);

// in FaultInjectionEngin
u8 ErrorListMutate(char **argv, u64 len, u8 *filemap, u8 *error_list);
void save_crashed_fu(const u8 *const error_list);
bool is_unique_fault(const u8 *const error_list);

// in DelayInsertionEngin
u8 control_matrix_mutate(char **argv, u64 len, u8 *filemap,
                         matrix_element *tmp_control_matrix);
u8 has_new_travel_matrix_element(matrix_element *tmp_travel_matrix,
                                 set<u64> &tmp_total_travel);
bool is_control_matrix_triggered(matrix_element *tmp_control_matrix,
                                 matrix_element *tmp_travel_matrix);

// in Setup
void GetCoreCount(void);
void BindFreeCPU(void);
void init_stats(void);

// in ForkServer
void init_forkserver(char **argv);
void call_forkserver(u32 &prev_time_out, s32 &child_pid);
void get_child_status(int &status);
