#define __MAIN__
#include "Fuzzer.h"

#define SYNC_CYCLE 1

seed_container queue;
seed_container::iterator queueCur;
seed_container::iterator queueTop[MAP_SIZE];
#ifndef NO_CONCURRENCY_FUZZ
seed_container::iterator concurrent_queue_top[MAP_SIZE];
#endif
u64 queueCycle = 0;
bool pendFavor = false;

u8 *globalTraceBit;
u8 *virginMap;
u8 *variableStateChangeCount;
u8 *branchTraceBit;
u8 *virginMapForBranchTraceBit;

u8 *concurrentFunctionCountVar;
u8 *tickNum;
pthread_mutex_t *concurrentFunctionMutex;

u32 *raw_thread_id_vec;
u32 *thread_id_vec;
u32 *thread_count;

bool isBranchCoverage = false;

// TODO: optimize to module
u8 *globalStateBit;

bitset<MAP_SIZE> timeoutTrace;
bitset<MAP_SIZE> crashTrace;
u32 unique_crash_count = 0;
u32 unique_hang_count = 0;
u32 total_crash_count = 0;
u32 total_timeout_count = 0;

u64 exec_count = 0;
u64 total_input = 0;
u64 usefule_input = 0;
u64 total_error_seq = 0;
u64 useful_error_seq = 0;

string testfile;
string target_path;
string inputDir;  // testcases
string outputDir; // fuzzing immediate result
s32 dev_null_fd;

string sync_id = "";
string sync_dir = "";
string sync_state = "";
bool skip_deterministic = true;
int sync_cov = 1;
u8 *sync_cov_ptr = NULL;
int sync_cov_length = MAP_SIZE; // can change
string sync_file_name = "sync_file";
string file_lock = "file_lock";
int master_sync_cycle = 5;
int slave_sync_cycle = 10;

u8 *sync_branch_cov_ptr = NULL;
string sync_branch_cov_file_name = "sync_branch_cov_file";

// u8 *errorMap;
u8 *errorList;
u8 *errorTraced;

#ifndef NO_CONCURRENCY_FUZZ
matrix_element *control_matrix;
matrix_element *adjacency_matrix;
matrix_element *travel_matrix;
set<u64> total_travel;
#endif
// u32 *delayList;
// u8 *delayTraced;
// u8 *concurrencyMap;
// u8 *virginConcurrencyMap;
// bitset<MAP_SIZE> concurrency_timeout_trace;
// bitset<MAP_SIZE> concurrency_crash_trace;

u64 timeBegin;

// bitset<MAP_SIZE> ex_timeout_trace;
u32 unique_ex_hang_count;
u32 total_ex_timeout_count;

u32 unique_ex_crash_count;

u32 no_forkserver = 1;
s32 forkserver_pid = 0;

u64 total_cal_us = 0;
u64 total_cal_cycles = 0;
u32 havoc_div = 1;

u8 timeout_given = 1;
u32 exec_tmout = EXEC_TIMEOUT;
u32 hang_tmout = EXEC_TIMEOUT;

void MakeDirectory(void)
{
    struct stat buffer;
    if (!sync_id.empty() && stat(sync_dir.c_str(), &buffer) != 0)
    { // use multiple fuzzer, and sync_dir does not exist
        cout << "sync_dir does not exist, mkdir " << sync_dir << endl;
        if (mkdir(sync_dir.c_str(), 0700))
        {
            cout << "fail to mkdir " << sync_dir << endl;
            exit(-1);
        }
    }

    if (stat(outputDir.c_str(), &buffer) == 0)
    {
        cout << "output folder does exist" << endl;
        string cmd = "rm " + outputDir + " -r";
        system(cmd.c_str());
    }

    string folder = outputDir;
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir " << outputDir << endl;
        exit(-1);
    }

    folder = outputDir + "queue/";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir queue" << endl;
        exit(-1);
    }
    folder = outputDir + "hang/";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir hang" << endl;
        exit(-1);
    }
    folder = outputDir + "crash/";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir crash" << endl;
        exit(-1);
    }
    folder = outputDir + "errorList/";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir errorList" << endl;
        exit(-1);
    }
    folder = outputDir + "delayList/";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir delayList" << endl;
        exit(-1);
    }
    folder = outputDir + "total_crash/";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir total_crash" << endl;
        exit(-1);
    }

    // midr unique ex_crash
    folder = outputDir + "ex_crash/";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir ex_crash" << endl;
        exit(-1);
    }
    folder = outputDir + "ex_crash/files";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir ex_crash/files" << endl;
        exit(-1);
    }
    folder = outputDir + "ex_crash/errorlists";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir ex_crash/errorlists" << endl;
        exit(-1);
    }
    folder = outputDir + "ex_crash/delaylists";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir ex_crash/delaylists" << endl;
        exit(-1);
    }

    // midr unique ex_hang
    folder = outputDir + "ex_hang/";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir fault_hang" << endl;
        exit(-1);
    }
    folder = outputDir + "ex_hang/files";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir ex_hang/files" << endl;
        exit(-1);
    }
    folder = outputDir + "ex_hang/errorlists";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir fault_hang/errorlists" << endl;
        exit(-1);
    }
    folder = outputDir + "ex_hang/delaylists";
    if (mkdir(folder.c_str(), 0700))
    {
        cout << "fail to mkdir fault_hang/delaylists" << endl;
        exit(-1);
    }

    if (!sync_id.empty())
    {
        folder = outputDir + ".synced/";
        if (mkdir(folder.c_str(), 0700))
        {
            cout << "fail to mkdir .synced" << endl;
            exit(-1);
        }
    }
}

s32 Init(int argc, char **argv)
{
    if (argc < 8)
    {
        cout << "you should input:\n"
             << "-i input_folder\n"
             << "-o output_folder\n"
             << "-f target_file\n"
             << "target_program" << endl;
        exit(-1);
    }

    s32 opt;
    while ((opt = getopt(argc, argv, "+f:i:o:M:S:b")) > 0)
    {
        switch (opt)
        {
        case 'f':
            if (!testfile.empty())
            {
                cout << "Multiple -f options not supported" << endl;
                exit(-1);
            }
            testfile = optarg;
            break;

        case 'i':
            if (!inputDir.empty())
            {
                cout << "Multiple -i options not supported" << endl;
                exit(-1);
            }
            inputDir = optarg;
            if (inputDir.back() != '/')
            {
                inputDir += '/';
            }
            break;

        case 'o':
            if (!outputDir.empty())
            {
                cout << "Multiple -o options not supported" << endl;
                exit(-1);
            }
            outputDir = optarg;
            if (outputDir.back() != '/')
            {
                outputDir += '/';
            }
            break;

        case 'M':
            if (!sync_id.empty())
            {
                cout << "Multiple -M or -S options not supported" << endl;
                exit(-1);
            }
            sync_id = optarg;
            sync_state = "MASTER";
            break;

        case 'S':
            if (!sync_id.empty())
            {
                cout << "Multiple -M or -S options not supported" << endl;
                exit(-1);
            }
            sync_id = optarg;
            sync_state = "SLAVE";
            break;
        case 'b':
            cout << "use branch coverage" << endl;
            isBranchCoverage = true;
            break;

        default:
            cout << "the opt " << opt << " is not supported" << endl;
            exit(-1);
        }
    }

    target_path = string(*(argv + optind));

    if (!sync_id.empty())
    {
        fix_up_sync(sync_id, sync_dir, outputDir);
    }

    dev_null_fd = open("/dev/null", O_RDWR);
    if (dev_null_fd < 0)
    {
        cout << "unable to open dev/null" << endl;
        exit(-1);
    }

    virginMap = new u8[MAP_SIZE];
    memset(virginMap, 255, MAP_SIZE * sizeof(u8));

    virginMapForBranchTraceBit = new u8[MAP_SIZE];
    memset(virginMapForBranchTraceBit, 255, MAP_SIZE * sizeof(u8));

    // virginConcurrencyMap = new u8[MAP_SIZE];
    // memset(virginConcurrencyMap, 255, MAP_SIZE * sizeof(u8));

    MakeDirectory();
    timeoutTrace.reset();
    crashTrace.reset();

    // ex_timeout_trace.reset();

    cout << "Init() success" << endl;
    return optind;
}

s32 Exit(void)
{
    // delete[] virginConcurrencyMap;
    delete[] virginMap;
    delete[] virginMapForBranchTraceBit;
    close(dev_null_fd);
    for (auto &q : queue)
    {
        if (q.trace != NULL)
        {
            delete q.trace;
        }
    }
    return 0;
}

#ifndef TEST
int main(int argc, char **argv)
{
    timeBegin = GetCurTimeUs();

    u64 i;
    u32 sync_cycle_cnt = 0;
    auto testArg = argv + Init(argc, argv);
    queue.clear();

    // bind to a free cpu (comment it when doing concurrency fuzzing)
    // GetCoreCount();
#ifdef __linux__
    // BindFreeCPU();
#endif
    print_result("Start");

    SetupSignalHandlers();
    cout << "SetupSignalHandlers() success" << endl;
    ShmSetup();
    cout << "ShmSetup() success" << endl;
    init_count_class();
    cout << "init_count_class() success" << endl;
    ReadTestcases(inputDir);
    cout << "ReadTestcases() success" << endl;

    if (sync_cov == 1 && !sync_id.empty())
    {
        if (sync_state == "MASTER")
        {
            sync_cov_ptr =
                multi_proc_shm_create(sync_file_name, sync_cov_length);
            sync_branch_cov_ptr = multi_proc_shm_create(
                sync_branch_cov_file_name, sync_cov_length);
        }
        else
        {
            sync_cov_ptr = multi_proc_shm_get(sync_file_name);
            sync_branch_cov_ptr = multi_proc_shm_get(sync_branch_cov_file_name);
        }
        if (access(file_lock.c_str(), 0) != 0)
        { // file_lock does not exist
            cout << file_lock << " does not exist, create it\n";
            ofstream f_lock(file_lock, ios::out);
            f_lock << "file_lock";
            f_lock.close();
        }
    }

    queueCur = queue.begin();
    for (i = 0; i < MAP_SIZE; i++)
    {
        queueTop[i] = queue.end();
#ifndef NO_CONCURRENCY_FUZZ
        concurrent_queue_top[i] = queue.end();
#endif
    }
    DryRunning(testArg);
    cout << "DryRunning() success" << endl;
    CullQueue();
    cout << "CullQueue() success" << endl;
    // for (auto q = queue.begin(); q != queue.end(); q++)ShowQueue(q);
    init_stats();

    cout << "queue.size = " << queue.size() << endl;
    while (true)
    {
        u8 stateFuzz;
        CullQueue();
        if (queueCur == queue.end())
        {
            queueCycle++;
            queueCur = queue.begin();
        }
        stateFuzz = FuzzMain(queueCur, testArg);
        queueCur++;

        if (sync_cov != 1)
        {
            if (!sync_id.empty() && !stateFuzz)
            {
                if (!(sync_cycle_cnt++ % SYNC_CYCLE))
                {
                    SychronizeFuzzer(testArg, sync_dir, sync_id);
                    cout << "Synchronize success" << endl;
                    cout << "queue.size = " << queue.size() << endl;
                }
            }
        }
    }
    ShmRemove();
    Exit();
    return 0;
}
#else
int main(int argc, char **argv)
{
    int status = 0;

    auto testArg = argv + 1;
    string target_path = argv[1];
    auto child_pid = fork();
    if (child_pid < 0)
    {
        cout << "fork() fail" << endl;
        exit(-1);
    }
    if (!child_pid)
    {
        // setsid();
        setenv("ASAN_OPTIONS",
               "abort_on_error=1:"
               "detect_leaks=0:"
               "symbolize=0:"
               "allocator_may_return_null=1",
               0);

        execv(target_path.c_str(), testArg);
        exit(0);
    }

    if (waitpid(child_pid, &status, 0) <= 0)
    {
        cout << "waitpid() fail" << endl;
        exit(-1);
    }

    if (WIFSIGNALED(status))
    {
        auto killSignal = WTERMSIG(status);
        if (killSignal == SIGKILL)
            return FAULT_TMOUT;
        cout << "\nFAULT_CRASH" << endl;
        return 0;
    }

    cout << "\nFAULT_NONE" << endl;
    return 0;
}
#endif
