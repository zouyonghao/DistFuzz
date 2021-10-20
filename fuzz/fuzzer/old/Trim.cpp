#include "Fuzzer.h"

#define TRIM_MIN_BYTES 4
#define TRIM_START_STEP 16
#define TRIM_END_STEP 1024

static inline u64 min_pow2(u64 len)
{
    u64 ret = 1;
    while (len > ret)
    {
        ret <<= 1;
    }
    return ret;
}

static void WriteGapTestcase(void *mem, u32 len, u32 skip_at, u32 skip_len, string testfile)
{
    unlink(testfile.c_str());
    s32 fd = open(testfile.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0600);
    u32 tail = len - skip_at - skip_len;
    if (fd < 0)
    {
        cout << "open() fail in WriteToTestcase, file is " << testfile << endl;
        exit(-1);
    }
    if (skip_at)
    {
        s32 res = write(fd, mem, skip_at);
        if (res != skip_at)
        {
            cout << "fail to write testcases in WriteGapTestcase" << endl;
            exit(-1);
        }
    }
    if (skip_len)
    {
        s32 res = write(fd, (const void *)((u64)mem + skip_at + skip_len), tail);
        if (res != tail)
        {
            cout << "fail to write testcases in WriteGapTestcase" << endl;
            exit(-1);
        }
    }
    close(fd);
}

u8 TrimCase(char **argv, seed_container::iterator qCur, u8 *inBuf)
{
    static u8 TrimTrace[MAP_SIZE];

    bool should_be_write = false;
    u8 fault = FAULT_NONE;
    u64 trim_size;
    u64 min_pow2_of_file_size;
    u64 trim_pos;
    u64 trim_avail_size;
    u32 cksum;
    u32 move_tail;

    if (qCur->fileLen < 5)
        return 0;

    min_pow2_of_file_size = min_pow2(qCur->fileLen);
    trim_size = max(min_pow2_of_file_size / TRIM_START_STEP, (u64)TRIM_MIN_BYTES);

    while (trim_size >= max(min_pow2_of_file_size / TRIM_END_STEP, (u64)TRIM_MIN_BYTES))
    {
        trim_pos = trim_size;

        while (trim_pos < qCur->fileLen)
        {
            trim_avail_size = min(trim_size, qCur->fileLen - trim_pos);
            WriteGapTestcase(inBuf, qCur->fileLen, trim_pos, trim_avail_size, testfile);
            fault = ExecuteCase(target_path, argv, exec_tmout);
            if (fault == FAULT_ERROR)
                return fault;

            cksum = hash32(globalTraceBit, MAP_SIZE, HASH_CONST);
            if (cksum == qCur->cksum)
            {
                move_tail = qCur->fileLen - trim_pos - trim_avail_size;
                qCur->fileLen -= trim_avail_size;
                min_pow2_of_file_size = min_pow2(qCur->fileLen);

                memmove(inBuf + trim_pos, inBuf + trim_pos + trim_avail_size, move_tail);
                if (!should_be_write)
                {
                    should_be_write = true;
                    memcpy(TrimTrace, globalTraceBit, MAP_SIZE);
                }
            }
            else
            {
                trim_pos += trim_size;
            }
        }

        trim_size >>= 1;
    }

    if (should_be_write)
    {
        s32 fd;
        s32 res;
        unlink(qCur->fileName.c_str());

        fd = open(qCur->fileName.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0600);
        if (fd < 0)
        {
            cout << "open() fail in TrimCase, file is " << qCur->fileName << endl;
            exit(-1);
        }

        res = write(fd, inBuf, qCur->fileLen);
        if (res != qCur->fileLen)
        {
            cout << "write() fail in TrimCase" << endl;
            exit(-1);
        }

        close(fd);

        memcpy(globalTraceBit, TrimTrace, MAP_SIZE);
        UpdateQueueTop(qCur, globalTraceBit);
    }
    return 0;
}