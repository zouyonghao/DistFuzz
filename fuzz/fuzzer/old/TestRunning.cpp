#include "Fuzzer.h"

#define CAL_CYCLES 1

#define ROL64(_x, _r) ((((u64)(_x)) << (_r)) | (((u64)(_x)) >> (64 - (_r))))

#define FF(_b) (0xff << ((_b) << 3))

static s32 child_pid;
static bool child_timed_out = false;
static u8 killSignal;

static u8 count_class_lookup8[256];
static u16 count_class_lookup16[65536];

bool are_errors_triggered(const u8 *const error_list, const u8 *const error_trace, u32 len)
{
    u32 i;
    for (i = 0; i < len; i++)
    {
        if (error_list[i] != 0 && error_trace[i] != 0)
            return true;
    }
    return false;
}

void ReadFile2Mem(const string &filename, u8 *mem, u32 len)
{
    auto fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0)
    {
        cout << "open " << filename << " fail in ReadFile2Mem" << endl;
        exit(-1);
    }

    auto res = read(fd, mem, len);
    if (res != len)
    {
        cout << "res(" << res << ") is not equal to len(" << len << ")" << endl;
        cout << "read " << filename << " fail in ReadFile2Mem" << endl;
        exit(-1);
    }
    close(fd);
}

void WriteMem2File(const string &filename, const u8 *const mem, u32 len)
{
    auto fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd < 0)
    {
        cout << "open() fail in WriteMem2File, cannot open " << filename << endl;
        exit(-1);
    }
    auto write_len = write(fd, mem, len);
    if (write_len != len)
    {
        cout << "write_len(" << write_len << ") is not equal to len(" << len << ")" << endl;
        cout << "write " << filename << " fail in WriteMem2File" << endl;
        cout << strerror(errno) << endl;
        exit(-1);
    }
    close(fd);
}

u32 hash32(const void *key, u32 len, u32 seed)
{
    const u64 *data = (u64 *)key;
    u64 h1 = seed ^ len;

    len >>= 3;

    while (len--)
    {
        u64 k1 = *data++;

        k1 *= 0x87c37b91114253d5ULL;
        k1 = ROL64(k1, 31);
        k1 *= 0x4cf5ad432745937fULL;

        h1 ^= k1;
        h1 = ROL64(h1, 27);
        h1 = h1 * 5 + 0x52dce729;
    }

    h1 ^= h1 >> 33;
    h1 *= 0xff51afd7ed558ccdULL;
    h1 ^= h1 >> 33;
    h1 *= 0xc4ceb9fe1a85ec53ULL;
    h1 ^= h1 >> 33;

    return h1;
}

void HandleTimeout(int sig)
{
    if (child_pid > 0)
    {
        child_timed_out = true;
        kill(child_pid, SIGKILL);
    }
}

void SetupSignalHandlers(void)
{
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_sigaction = NULL;
    sigemptyset(&sa.sa_mask);

    sa.sa_handler = HandleTimeout;
    sigaction(SIGALRM, &sa, NULL);
}

void WriteToTestcase(void *mem, u32 len, const string &current_testfile)
{
    unlink(current_testfile.c_str());
    s32 fd = open(current_testfile.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd < 0)
    {
        cout << "open() fail in WriteToTestcase, file is " << current_testfile << endl;
        exit(-1);
    }
    s32 res = write(fd, mem, len);
    if (res != len)
    {
        cout << "fail to write testcases, res < len" << endl;
        exit(-1);
    }
    close(fd);
}

void init_count_class(void)
{
    // Initialize count_class_lookup8
    u32 i;
    count_class_lookup8[0] = 0;
    count_class_lookup8[1] = 1;
    count_class_lookup8[2] = 2;
    count_class_lookup8[3] = 4;
    for (i = 4; i < 8; i++)
        count_class_lookup8[i] = 8;
    for (i = 8; i < 16; i++)
        count_class_lookup8[i] = 16;
    for (i = 16; i < 32; i++)
        count_class_lookup8[i] = 32;
    for (i = 32; i < 128; i++)
        count_class_lookup8[i] = 64;
    for (i = 128; i < 256; i++)
        count_class_lookup8[i] = 128;

    // Initialize count_class_lookup16
    u32 b1, b2;
    for (b1 = 0; b1 < 256; b1++)
        for (b2 = 0; b2 < 256; b2++)
            count_class_lookup16[(b1 << 8) + b2] = (count_class_lookup8[b1] << 8) | count_class_lookup8[b2];
}

#ifdef __x86_64__

static inline void classify_counts(u64 *mem)
{
    u32 i = MAP_SIZE >> 3;
    while (i--)
    {
        /* Optimize for sparse bitmaps. */
        if (*mem)
        {
            u16 *mem16 = (u16 *)mem;

            mem16[0] = count_class_lookup16[mem16[0]];
            mem16[1] = count_class_lookup16[mem16[1]];
            mem16[2] = count_class_lookup16[mem16[2]];
            mem16[3] = count_class_lookup16[mem16[3]];
        }
        mem++;
    }
}

#else

static inline void classify_counts(u32 *mem)
{
    u32 i = MAP_SIZE >> 2;
    while (i--)
    {
        /* Optimize for sparse bitmaps. */
        if (*mem)
        {
            u16 *mem16 = (u16 *)mem;

            mem16[0] = count_class_lookup16[mem16[0]];
            mem16[1] = count_class_lookup16[mem16[1]];
        }
        mem++;
    }
}

#endif /* ^__x86_64__ */

u8 ExecuteCase(const string &current_target_path, char **argv, u32 timeout)
{
    static itimerval itimer;
    static u32 prev_timed_out = 0;

    int status = 0;
    child_timed_out = false;

    *((u32 *)variableStateChangeCount) = 0;
    *((u64 *)concurrentFunctionCountVar) = 0;
    // *((u32 *)thread_count) = 0;
    *((u64 *)tickNum) = 0;
    pthread_mutex_unlock(multiProcessMutex);
    cout << "Execute !!" << endl;
    memset(globalTraceBit, 0, MAP_SIZE * sizeof(u8));
    memset(errorTraced, 0, MAX_LIST_SIZE * sizeof(u8));
    memset(branchTraceBit, 0, MAP_SIZE * sizeof(u8));
#ifndef NO_CONCURRENCY_FUZZ
    memset(adjacency_matrix, 0, ADJACENCY_MATRIX_SIZE * sizeof(matrix_element));
    memset(travel_matrix, 0, TRAVEL_MATRIX_SIZE * sizeof(matrix_element));
#endif

    if (no_forkserver)
    {
        child_pid = fork();
        if (child_pid < 0)
        {
            cout << "fork() fail" << endl;
            exit(-1);
        }
        if (!child_pid)
        { // child process
            // TODO: resource limit
            // 		 environment variable

            setsid();
            // dup2(dev_null_fd, 0);
            // dup2(dev_null_fd, 1);
            // dup2(dev_null_fd, 2);

            close(dev_null_fd);

            setenv("ASAN_OPTIONS",
                   "abort_on_error=1:"
                   "detect_leaks=0:"
                   "symbolize=0:"
                   "allocator_may_return_null=1",
                   0);

            // usleep(995000);
            execv(current_target_path.c_str(), argv);
            exit(0);
        }
    }
    else
    {
        call_forkserver(prev_timed_out, child_pid);
    }

    // timeout is ms
    itimer.it_value.tv_sec = timeout / 1000;           // s limit
    itimer.it_value.tv_usec = (timeout % 1000) * 1000; // us limit

    setitimer(ITIMER_REAL, &itimer, NULL);

    if (no_forkserver)
    {
        if (waitpid(child_pid, &status, 0) <= 0)
        {
            cout << "waitpid() fail" << endl;
            exit(-1);
        }
    }
    else
    {
        get_child_status(status);
    }

    if (!WIFSTOPPED(status))
        child_pid = 0;
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itimer, NULL);

#ifdef __x86_64__
    classify_counts((u64 *)globalTraceBit);
#else
    classify_counts((u32 *)globalTraceBit);
#endif /* ^__x86_64__ */

    prev_timed_out = child_timed_out;

    exec_count += 1;
    if (WIFSIGNALED(status))
    {
        // printf("status num: %d\n", status);
        killSignal = WTERMSIG(status);
        if (child_timed_out && killSignal == SIGKILL)
            return FAULT_TMOUT;
        return FAULT_CRASH;
    }

    // cout << "VariableStateChangeCount: " << *((u32
    // *)variableStateChangeCount) << endl;
    return FAULT_NONE;
}

u64 GetCurTimeUs(void)
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    return (tv.tv_sec * 1000000ULL) + tv.tv_usec;
}

u8 CalibrateCase(char **argv, seed_container::iterator q, u8 *mem)
{
    u32 i;
    u32 cksum;
    u8 run_status;
    u32 use_tmout = exec_tmout;

    if (!no_forkserver && !forkserver_pid)
    {
        init_forkserver(argv);
    }

    u64 time_begin = GetCurTimeUs();
    for (i = 0; i < CAL_CYCLES; i++)
    {
        WriteToTestcase(mem, q->fileLen, testfile);
        run_status = ExecuteCase(target_path, argv, use_tmout);

        if (run_status != FAULT_NONE)
        {
            // printf("run_status error: %d\n", run_status);
            u64 time_end = GetCurTimeUs();
            total_cal_us += (time_end - time_begin);
            total_cal_cycles += (i + 1);
            return run_status;
        }

        if (q->cksum == 0)
        {
            q->cksum = hash32(globalTraceBit, MAP_SIZE, HASH_CONST);
        }
    }
    u64 time_end = GetCurTimeUs();

    total_cal_us += (time_end - time_begin);
    total_cal_cycles += CAL_CYCLES;

    q->execTime = (time_end - time_begin) / CAL_CYCLES;
    UpdateQueueTop(q, globalTraceBit);
    return run_status;
}

static u32 CountBytes(u8 *mem)
{
    u32 *ptr = (u32 *)mem;
    u32 i = (MAP_SIZE >> 2);
    u32 ret = 0;

    while (i--)
    {
        u32 v = *(ptr++);

        if (!v)
            continue;
        if (v & FF(0))
            ret++;
        if (v & FF(1))
            ret++;
        if (v & FF(2))
            ret++;
        if (v & FF(3))
            ret++;
    }

    return ret;
}

void CheckMapSize(void)
{
    u32 i;
    u32 bytecounted = CountBytes(globalTraceBit);
    if (bytecounted > MAP_SIZE / 10)
    {
        cout << "bytecounted = " << bytecounted << ", map maybe too small" << endl;
    }
}

void ShowQueue(seed_container::iterator q)
{
    cout << "-----------------" << endl;
    cout << "q->wasFuzzed = " << (q->wasFuzzed ? "True" : "False") << endl;
    cout << "q->favored = " << (q->favored ? "True" : "False") << endl;
    cout << "q->fileName = " << q->fileName << endl;
    cout << "q->fileLen = " << q->fileLen << endl;
    cout << "q->execTime = " << q->execTime << endl;
    cout << (q->trace == NULL ? "not trace" : "has trace") << endl;
    cout << "q->topCount = " << q->topCount << endl;
    cout << "q->cksum = " << q->cksum << endl;
    cout << "-----------------" << endl;
}

void DryRunning(char **argv)
{
    s32 fd;
    u8 res;
    for (auto q = queue.begin(); q != queue.end(); q++)
    {
        unique_ptr<u8[]> mem(new u8[q->fileLen]);
        fd = open(q->fileName.c_str(), O_RDONLY);
        if (fd < 0)
        {
            cout << "open() fail in DryRunning" << endl;
            exit(-1);
        }

        // mem = new u8[q->fileLen];
        if (read(fd, mem.get(), q->fileLen) < 0)
        {
            cout << "read() fail in DryRunning" << endl;
            exit(-1);
        }
        close(fd);

        res = CalibrateCase(argv, q, mem.get());
        // delete mem;    use unique_ptr instead
        if (q == queue.begin())
            CheckMapSize();
    }
}

static u8 has_new_bits(u8 *globalTraceBit, u8 *virgin_map)
{
#ifdef __x86_64__

    u64 *current = (u64 *)globalTraceBit;
    u64 *virgin = (u64 *)virgin_map;

    u32 i = (MAP_SIZE >> 3);

#else

    u32 *current = (u32 *)globalTraceBit;
    u32 *virgin = (u32 *)virgin_map;

    u32 i = (MAP_SIZE >> 2);

#endif /* ^__x86_64__ */

    u8 ret = 0;

    while (i--)
    {
        if ((*current) && (*current & *virgin))
        {
            if ((ret < 2))
            {
                u8 *cur = (u8 *)current;
                u8 *vir = (u8 *)virgin;

#ifdef __x86_64__

                if ((cur[0] && vir[0] == 0xff) || (cur[1] && vir[1] == 0xff) || (cur[2] && vir[2] == 0xff) ||
                    (cur[3] && vir[3] == 0xff) || (cur[4] && vir[4] == 0xff) || (cur[5] && vir[5] == 0xff) ||
                    (cur[6] && vir[6] == 0xff) || (cur[7] && vir[7] == 0xff))
                    ret = 2;
                else
                    ret = 1;

#else

                if ((cur[0] && vir[0] == 0xff) || (cur[1] && vir[1] == 0xff) || (cur[2] && vir[2] == 0xff) ||
                    (cur[3] && vir[3] == 0xff))
                    ret = 2;
                else
                    ret = 1;

#endif /* ^__x86_64__ */
            }

            *virgin &= ~*current;
        }

        current++;
        virgin++;
    }

    return ret;
}

string num2str_Keep6bit(u32 num)
{
    char ss[10];
    sprintf(ss, "%06u", num);
    return ss;
}

u8 crash_handler(u8 keeping, u8 *mem, u32 len)
{
    seed_container::iterator q;
    bitset<MAP_SIZE> trace;
    bitset<MAP_SIZE> trace2;
    string filename;

    total_crash_count++;
    filename = outputDir + "total_crash/" + num2str_Keep6bit(total_crash_count) + "-" + to_string(GetCurTimeUs());
    WriteMem2File(filename, mem, len);

    bool new_bit = false;
    bool new_concurrency_bit = false;

    MinimizeBit(&trace, globalTraceBit);
    trace2 = trace & (~crashTrace);
    new_bit = !trace2.none();

#ifndef NO_CONCURRENCY_FUZZ
    // TODO: add concurrency crash handling
    // bitset<MAP_SIZE> concurrency_trace;
    // bitset<MAP_SIZE> concurrency_trace2;
    // MinimizeBit(&concurrency_trace, concurrencyMap);
    // concurrency_trace2 = concurrency_trace & (~concurrency_crash_trace);
    // new_concurrency_bit = !concurrency_trace2.none();
#endif

    if (!new_bit && !new_concurrency_bit)
        return keeping;

    if (new_bit)
        crashTrace |= trace;

#ifndef NO_CONCURRENCY_FUZZ
        // if (new_concurrency_bit)
        // 	concurrency_crash_trace |= concurrency_trace2;
#endif

    unique_crash_count++;

    filename = outputDir + "crash/" + num2str_Keep6bit(unique_crash_count) + "-" + num2str_Keep6bit(total_crash_count) +
               "-" + to_string(GetCurTimeUs());
    WriteMem2File(filename, mem, len);

    if (!keeping)
        usefule_input++;

    return 1;
}

u8 ex_crash_handler(u8 keeping, u8 *mem, u32 len)
{
    string filename;
    string error_filename;
    string delay_filename;
    string crash_name;

    bool new_bit = false;
    bool new_concurrency_bit = false;
    bool unique_fault = false;

    // check if fault injection is unique
    unique_fault = is_unique_fault(errorList);

    // check if traditional coverage is unique
    bitset<MAP_SIZE> trace;
    bitset<MAP_SIZE> trace2;
    MinimizeBit(&trace, globalTraceBit);
    trace2 = trace & (~crashTrace);
    new_bit = !trace2.none();

    // check if concurrency coverage is unique
#ifndef NO_CONCURRENCY_FUZZ
    // TODO: add concurrency crash handling
    // bitset<MAP_SIZE> concurrency_trace;
    // bitset<MAP_SIZE> concurrency_trace2;
    // MinimizeBit(&concurrency_trace, concurrencyMap);
    // concurrency_trace2 = concurrency_trace & (~concurrency_crash_trace);
    // new_concurrency_bit = !concurrency_trace2.none();
#endif

    // No --> since the fuzzed test is the unique fu (in ErrorListMutate), so
    // the crash is unique naturally bitflip will cause over counted
    if (!unique_fault && !new_bit && !new_concurrency_bit)
        return keeping;

    // save the crashed fu
    if (unique_fault)
        save_crashed_fu(errorList);
    if (new_bit)
        crashTrace |= trace;
#ifndef NO_CONCURRENCY_FUZZ
        // if (new_concurrency_bit)
        // 	concurrency_crash_trace |= concurrency_trace2;
#endif

    // write fault unique crash folder
    unique_ex_crash_count++;

    crash_name = num2str_Keep6bit(unique_ex_crash_count) + "-" + to_string(GetCurTimeUs());

    filename = outputDir + "ex_crash/files/" + crash_name;
    WriteMem2File(filename, mem, len);

    if (are_errors_triggered(errorList, errorTraced, MAX_LIST_SIZE))
    {
        error_filename = outputDir + "ex_crash/errorlists/" + crash_name;
        WriteMem2File(error_filename, errorList, MAX_LIST_SIZE);
    }

#ifndef NO_CONCURRENCY_FUZZ
    // if (are_delays_triggered(delayList, delayTraced, MAX_LIST_SIZE)) {
    // 	delay_filename = outputDir + "ex_crash/delaylists/" + crash_name;
    // 	WriteMem2File(delay_filename, (u8 *)delayList, MAX_LIST_SIZE *
    // sizeof(u32) / sizeof(u8));
    // }
#endif

    if (!keeping && unique_fault)
        useful_error_seq++;

    return 1;
}

u8 hang_handler(u8 keeping, char **argv, u8 *mem, u32 len)
{
    seed_container::iterator q;

    string filename;

    total_timeout_count++;

    bool new_bit = false;
    bool new_concurrency_bit = false;

    // check if traditional coverage is unique
    bitset<MAP_SIZE> trace;
    bitset<MAP_SIZE> trace2;
    MinimizeBit(&trace, globalTraceBit);
    trace2 = trace & (~timeoutTrace);
    new_bit = !trace2.none();

#ifndef NO_CONCURRENCY_FUZZ
    // check if concurrency coverage is unique
    // TODO: add concurrency crash handling
    // bitset<MAP_SIZE> concurrency_trace;
    // bitset<MAP_SIZE> concurrency_trace2;
    // MinimizeBit(&concurrency_trace, concurrencyMap);
    // concurrency_trace2 = concurrency_trace & (~concurrency_timeout_trace);
    // new_concurrency_bit = !concurrency_trace2.none();
#endif

    if (!new_bit && !new_concurrency_bit)
        return keeping;

    if (exec_tmout < hang_tmout)
    {
        WriteToTestcase(mem, len, testfile);
        auto new_status = ExecuteCase(target_path, argv, hang_tmout);
        if (new_status == FAULT_CRASH)
        {
            keeping = crash_handler(keeping, mem, len);
            return keeping;
        }

        if (new_status != FAULT_TMOUT)
            return keeping;
    }

    if (new_bit)
        timeoutTrace |= trace;

#ifndef NO_CONCURRENCY_FUZZ
        // if (new_concurrency_bit)
        // 	concurrency_timeout_trace |= concurrency_trace;
#endif

    unique_hang_count++;

    filename = outputDir + "hang/" + num2str_Keep6bit(unique_hang_count) + "-" + to_string(GetCurTimeUs());
    WriteMem2File(filename, mem, len);

    if (!keeping)
        usefule_input++;

    return 1;
}

u8 ex_hang_handler(u8 keeping, char **argv, u8 *mem, u32 len)
{
    seed_container::iterator q;
    bitset<MAP_SIZE> trace;
    bitset<MAP_SIZE> trace2;
    string filename;
    string error_filename;
    string delay_filename;
    string hang_name;

    total_ex_timeout_count++;

    bool new_bit = false;
    bool new_concurrency_bit = false;

    MinimizeBit(&trace, globalTraceBit);
    trace2 = trace & (~timeoutTrace);
    new_bit = !trace2.none();

#ifndef NO_CONCURRENCY_FUZZ
    // bitset<MAP_SIZE> concurrency_trace;
    // bitset<MAP_SIZE> concurrency_trace2;
    // MinimizeBit(&concurrency_trace, concurrencyMap);
    // concurrency_trace2 = concurrency_trace & (~concurrency_timeout_trace);
    // new_concurrency_bit = !concurrency_trace2.none();
#endif

    if (!new_bit && !new_concurrency_bit)
        return keeping;

    // check whether it is a real hang (give more time)
    if (exec_tmout < hang_tmout)
    {
        // errorList has been set
        WriteToTestcase(mem, len, testfile);
        auto new_status = ExecuteCase(target_path, argv, hang_tmout);

        if (new_status == FAULT_CRASH)
        {
            keeping = ex_crash_handler(keeping, mem, len);
            return keeping;
        }

        if (new_status != FAULT_TMOUT)
            return keeping;
    }

    if (new_bit)
        timeoutTrace |= trace;

#ifndef NO_CONCURRENCY_FUZZ
        // if (new_concurrency_bit)
        // 	concurrency_timeout_trace |= concurrency_trace;
#endif

    unique_ex_hang_count++;

    hang_name = num2str_Keep6bit(unique_ex_hang_count) + "-" + to_string(GetCurTimeUs());

    filename = outputDir + "ex_hang/files/" + hang_name;
    WriteMem2File(filename, mem, len);

    if (are_errors_triggered(errorList, errorTraced, MAX_LIST_SIZE))
    {
        error_filename = outputDir + "ex_hang/errorlists/" + hang_name;
        WriteMem2File(error_filename, errorList, MAX_LIST_SIZE);
    }

#ifndef NO_CONCURRENCY_FUZZ
    // if (are_delays_triggered(delayList, delayTraced, MAX_LIST_SIZE)) {
    // 	delay_filename = outputDir + "ex_hang/delaylists/" + hang_name;
    // 	WriteMem2File(delay_filename, (u8 *)delayList, MAX_LIST_SIZE *
    // sizeof(u32) / sizeof(u8));
    // }
#endif

    if (!keeping)
        useful_error_seq++;

    return 1;
}

u8 AddInterestingSeed(char **argv, u8 *mem, u32 len, u8 run_status)
{
    u8 newBitFlag = 0;
    u8 new_travel_element = 0;
    string filename;
    string error_list_filename;
    string control_matrix_filename;
    u8 keeping = 0;
    seed_container::iterator q;
    bitset<MAP_SIZE> trace;
    bitset<MAP_SIZE> trace2;

    u8 branchBitFlag = has_new_bits(branchTraceBit, virginMapForBranchTraceBit);
    newBitFlag = has_new_bits(globalTraceBit, virginMap);
    // cout << CountBytes(globalTraceBit) << endl;
    if (isBranchCoverage)
    {
        if (!newBitFlag)
        {
            newBitFlag |= branchBitFlag;
        }
        // newBitFlag = branchBitFlag;
    }
    // printf("newBitFlag %d\n", newBitFlag);
#ifndef NO_CONCURRENCY_FUZZ
    new_travel_element = has_new_travel_matrix_element(travel_matrix, total_travel);
#endif

    bool eflag = false;
    bool cflag = false;
#ifndef NO_FUZZING_ERROR
    eflag = are_errors_triggered(errorList, errorTraced, MAX_LIST_SIZE);
#endif
#ifndef NO_CONCURRENCY_FUZZ
    cflag = is_control_matrix_triggered(control_matrix, travel_matrix);
#endif

    if (newBitFlag || new_travel_element)
    { // is interesting
        filename = outputDir + "queue/" + num2str_Keep6bit(queue.size()) + (newBitFlag == 2 ? "+cov" : "+tup");

        char *real_error_list_filename = NULL;
        char *real_control_matrix_filename = NULL;

#ifndef NO_FUZZING_ERROR
        if (eflag)
        {
            error_list_filename =
                outputDir + "errorList/" + num2str_Keep6bit(queue.size()) + (newBitFlag == 2 ? "+cov" : "+tup");
            real_error_list_filename = (char *)malloc(error_list_filename.size() + 5);
            strcpy(real_error_list_filename, error_list_filename.c_str());

            WriteMem2File(error_list_filename, errorList, MAX_LIST_SIZE);
            useful_error_seq++;
        }
#endif
#ifndef NO_CONCURRENCY_FUZZ
        if (cflag)
        {
            control_matrix_filename =
                outputDir + "delayList/" + num2str_Keep6bit(queue.size()) + (newBitFlag == 2 ? "+cov" : "+tup");
            real_control_matrix_filename = (char *)malloc(control_matrix_filename.size() + 5);
            strcpy(real_control_matrix_filename, control_matrix_filename.c_str());

            WriteMem2File(control_matrix_filename, (u8 *)control_matrix,
                          CONTROL_MATRIX_SIZE * sizeof(matrix_element) / sizeof(u8));
        }
#endif
        if (!eflag && !cflag)
            usefule_input++;

        AddToQueue(filename.c_str(), len, real_error_list_filename, real_control_matrix_filename);
        WriteMem2File(filename, mem, len);

        if (real_error_list_filename)
            free(real_error_list_filename);
        if (real_control_matrix_filename)
            free(real_control_matrix_filename);

        q = --queue.end();
        q->cksum = hash32(globalTraceBit, MAP_SIZE, HASH_CONST);
        // auto res = CalibrateCase(argv, q, mem);
        q->execTime = 1;
        keeping = 1;
    }

    auto ex_flag = eflag || cflag;
    switch (run_status)
    {
    case FAULT_TMOUT:
        if (ex_flag)
        {
            keeping = ex_hang_handler(keeping, argv, mem, len);
        }
        else
        {
            keeping = hang_handler(keeping, argv, mem, len);
        }
        break;

    case FAULT_CRASH:
        if (ex_flag)
        {
            keeping = ex_crash_handler(keeping, mem, len);
        }
        else
        {
            keeping = crash_handler(keeping, mem, len);
        }
        break;

    default:
        return keeping;
    }

    return keeping;
}

u8 FuzzUnit(char **argv, u8 *filemap, u64 len, u8 *error_list, matrix_element *tmp_cm)
{
    u8 run_status;
    static int fuzz_time = 0, sync_cycle;

    if (sync_state == "MASTER")
        sync_cycle = master_sync_cycle;
    else
        sync_cycle = slave_sync_cycle;

    if (sync_cov == 1)
    {
        fuzz_time = (fuzz_time + 1) % sync_cycle;
        if (fuzz_time == 0)
        {
            cout << "syncing...";
            FILE *fp = fopen(file_lock.c_str(), "r+b");
            if (fp == NULL)
            {
                cout << "open " << file_lock << " fail in FuzzUnit()" << endl;
                exit(-1);
            }
            flock(fp->_fileno, LOCK_EX);

            sync_coverage(virginMap, sync_cov_ptr, MAP_SIZE);
            sync_coverage(virginMapForBranchTraceBit, sync_branch_cov_ptr, MAP_SIZE);

            flock(fp->_fileno, LOCK_UN);
            fclose(fp);
            cout << "success" << endl;
        }
    }

    // if error list is null, the errorList is the error list used lastly
    if (error_list != NULL)
    {
        total_error_seq++;
        memcpy(errorList, error_list, MAX_LIST_SIZE * sizeof(u8));
    }
    else
        total_input++;

#ifndef NO_CONCURRENCY_FUZZ
    // if delay list is null, the delayList is the delay list used lastly
    if (tmp_cm != NULL)
        memcpy(control_matrix, tmp_cm, CONTROL_MATRIX_SIZE * sizeof(matrix_element));
#endif

    WriteToTestcase(filemap, len, testfile);
    run_status = ExecuteCase(target_path, argv, exec_tmout);
    // if (run_status == FAULT_CRASH)
    // {
        // cout << "Crash, run again" << endl;
        // cout << "catch a crash!!" << endl;
        // exit(-1);
        // usleep(200000);
        // run_status = ExecuteCase(target_path, argv, exec_tmout);
        // cout << "Crash again, run again" << endl;
        // if (run_status == FAULT_CRASH) {
        // 	usleep(4000000);
        // 	run_status = ExecuteCase(target_path, argv, exec_tmout);
        // }
    // }

    // cout << "run_status = " << (int)run_status << endl;
    AddInterestingSeed(argv, filemap, len, run_status);

    // if (run_status == FAULT_CRASH) {
    // 	cout << "==============================" << endl;
    // 	cout << "catch a crash, stop!!" << endl;
    // 	exit(-1);
    // }
    print_result("temp stage");
    return 0;
}
