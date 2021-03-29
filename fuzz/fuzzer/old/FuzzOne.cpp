#include "Fuzzer.h"

#define FLIP_BIT(_ar, _b)                                                      \
    do                                                                         \
    {                                                                          \
        u8 *_arf = (u8 *)(_ar);                                                \
        u32 _bf = (_b);                                                        \
        _arf[(_bf) >> 3] ^= (128 >> ((_bf)&7));                                \
    } while (0)

#define SWAP16(_x)                                                             \
    ({                                                                         \
        u16 _ret = (_x);                                                       \
        (u16)((_ret << 8) | (_ret >> 8));                                      \
    })

#define SWAP32(_x)                                                             \
    ({                                                                         \
        u32 _ret = (_x);                                                       \
        (u32)((_ret << 24) | (_ret >> 24) | ((_ret << 8) & 0x00FF0000) |       \
              ((_ret >> 8) & 0x0000FF00));                                     \
    })

#define INTERESTING_8                                                          \
    -128,    /* Overflow signed 8-bit when decremented  */                     \
        -1,  /*                                         */                     \
        0,   /*                                         */                     \
        1,   /*                                         */                     \
        16,  /* One-off with common buffer size         */                     \
        32,  /* One-off with common buffer size         */                     \
        64,  /* One-off with common buffer size         */                     \
        100, /* One-off with common buffer size         */                     \
        127  /* Overflow signed 8-bit when incremented  */

#define INTERESTING_16                                                         \
    -32768,   /* Overflow signed 16-bit when decremented */                    \
        -129, /* Overflow signed 8-bit                   */                    \
        128,  /* Overflow signed 8-bit                   */                    \
        255,  /* Overflow unsig 8-bit when incremented   */                    \
        256,  /* Overflow unsig 8-bit                    */                    \
        512,  /* One-off with common buffer size         */                    \
        1000, /* One-off with common buffer size         */                    \
        1024, /* One-off with common buffer size         */                    \
        4096, /* One-off with common buffer size         */                    \
        32767 /* Overflow signed 16-bit when incremented */

#define INTERESTING_32                                                         \
    -2147483648LL,  /* Overflow signed 32-bit when decremented */              \
        -100663046, /* Large negative number (endian-agnostic) */              \
        -32769,     /* Overflow signed 16-bit                  */              \
        32768,      /* Overflow signed 16-bit                  */              \
        65535,      /* Overflow unsig 16-bit when incremented  */              \
        65536,      /* Overflow unsig 16 bit                   */              \
        100663045,  /* Large positive number (endian-agnostic) */              \
        2147483647  /* Overflow signed 32-bit when incremented */

#define MIN_TOKEN_SIZE 1
#define MAX_TOKEN_SIZE 32
#define MAX_TOKEN_NUM 50

#define MAX_FILE_LEN (1 * 1024 * 1024)
#define MIN_FILE_LEN_FOR_USEFUL 128

#define HAVOC_CYCLES_INIT 1024
#define HAVOC_STACK_POW2 7

#define HAVOC_BLK_SMALL 32
#define HAVOC_BLK_MEDIUM 128
#define HAVOC_BLK_LARGE 1500
#define HAVOC_BLK_XL 32768
#define HAVOC_INIT_MULT 4
#define HAVOC_MAX_MULT 16

#define SPLICE_CYCLES 15
#define SPLICE_HAVOC 32

static s8 interesting_8[] = {INTERESTING_8};
static s16 interesting_16[] = {INTERESTING_8, INTERESTING_16};
static s32 interesting_32[] = {INTERESTING_8, INTERESTING_16, INTERESTING_32};

static bool *useful_byte = 0; // useful:1  useless:0
static map<string, u32> autoToken;
static map<string, u32> pointedToken;

string GetTimeStr(void);

static u8 could_be_bitflip(u32 xor_val)
{
    u32 sh = 0;
    if (!xor_val)
        return 1;

    while (!(xor_val & 1))
    {
        sh++;
        xor_val >>= 1;
    }

    if (xor_val == 1 || xor_val == 3 || xor_val == 15)
        return 1;

    if (sh & 7)
        return 0;

    if (xor_val == 0xff || xor_val == 0xffff || xor_val == 0xffffffff)
        return 1;

    return 0;
}

static u8 could_be_arith(u32 old_val, u32 new_val, u8 blen)
{
    u32 i, ov = 0, nv = 0, diffs = 0;

    if (old_val == new_val)
        return 1;

    for (i = 0; i < blen; i++)
    {
        u8 a = old_val >> (8 * i), b = new_val >> (8 * i);
        if (a != b)
        {
            diffs++;
            ov = a;
            nv = b;
        }
    }

    if (diffs == 1)
    {
        if ((u8)(ov - nv) <= ARITH_MAX || (u8)(nv - ov) <= ARITH_MAX)
            return 1;
    }

    if (blen == 1)
        return 0;

    diffs = 0;

    for (i = 0; i < blen / 2; i++)
    {
        u16 a = old_val >> (16 * i), b = new_val >> (16 * i);
        if (a != b)
        {
            diffs++;
            ov = a;
            nv = b;
        }
    }

    if (diffs == 1)
    {
        if ((u16)(ov - nv) <= ARITH_MAX || (u16)(nv - ov) <= ARITH_MAX)
            return 1;

        ov = SWAP16(ov);
        nv = SWAP16(nv);

        if ((u16)(ov - nv) <= ARITH_MAX || (u16)(nv - ov) <= ARITH_MAX)
            return 1;
    }

    if (blen == 4)
    {
        if ((u32)(old_val - new_val) <= ARITH_MAX ||
            (u32)(new_val - old_val) <= ARITH_MAX)
            return 1;

        new_val = SWAP32(new_val);
        old_val = SWAP32(old_val);

        if ((u32)(old_val - new_val) <= ARITH_MAX ||
            (u32)(new_val - old_val) <= ARITH_MAX)
            return 1;
    }

    return 0;
}

static u8 could_be_interest(u32 old_val, u32 new_val, u8 blen, u8 check_le)
{
    u32 i, j;

    if (old_val == new_val)
        return 1;

    for (i = 0; i < blen; i++)
    {
        for (j = 0; j < sizeof(interesting_8) / sizeof(s8); j++)
        {
            u32 tval = (old_val & ~(0xff << (i * 8))) |
                       (((u8)interesting_8[j]) << (i * 8));

            if (new_val == tval)
                return 1;
        }
    }

    if (blen == 2 && !check_le)
        return 0;

    for (i = 0; i < blen - 1; i++)
    {
        for (j = 0; j < sizeof(interesting_16) / sizeof(s16); j++)
        {
            u32 tval = (old_val & ~(0xffff << (i * 8))) |
                       (((u16)interesting_16[j]) << (i * 8));

            if (new_val == tval)
                return 1;

            if (blen > 2)
            {
                tval = (old_val & ~(0xffff << (i * 8))) |
                       (SWAP16(interesting_16[j]) << (i * 8));

                if (new_val == tval)
                    return 1;
            }
        }
    }

    if (blen == 4 && check_le)
    {
        for (j = 0; j < sizeof(interesting_32) / sizeof(s32); j++)
            if (new_val == (u32)interesting_32[j])
                return 1;
    }

    return 0;
}

static inline u32 randNum(u32 denominator)
{
    static u32 count = 0;
    if (count >= 10000)
    {
        srand((int)time(0));
        count = 0;
    }
    if (denominator == 0)
    {
        return 0;
    }
    return rand() % denominator;
}

u32 ShowCoverage(u8 *virgin_map)
{
    u32 i;
    u32 cov_count = 0;
    for (i = 0; i < MAP_SIZE; i++)
    {
        if (virgin_map[i] != 255)
            cov_count++;
    }
    return cov_count;
}

void print_result(string stage)
{
    ofstream output;
    string filename = outputDir + "plot-curve";
    output.open(filename, ios::app);
    output << GetCurTimeUs() << "   ";
    output << ShowCoverage(virginMap) << "("
           << 1.0 * ShowCoverage(virginMap) * 100 / MAP_SIZE << "%)"
           << "   ";
    output << unique_crash_count << "/" << total_crash_count << "   ";
    output << unique_ex_crash_count << "   ";

    output << "bc:" << ShowCoverage(virginMapForBranchTraceBit) << "("
           << 1.0 * ShowCoverage(virginMapForBranchTraceBit) * 100 / MAP_SIZE
           << "%)"
           << "   ";
    // output << useful_error_seq << "/" << total_error_seq << "   ";
    // output << usefule_input << "/" << total_input << "   ";
#ifndef NO_CONCURRENCY_FUZZ
    output << total_travel.size() << " ";
    output << concurrency_test_time << " ";
    output << unreachable_control_matrix.size() << " ";
    output << skip_since_travel << " ";
    output << skip_since_unreachable << " ";
#endif
    output << queue.size() << " ";
    output << stage << " ";
    output << "ec: " << exec_count << "  ";
    output << GetTimeStr() << endl;
    output.close();
}

static void AddToken(string token)
{
    u32 i;
    auto len = token.size();
    u8 *data = (u8 *)token.c_str();
    if (len == 2)
    {
        i = sizeof(interesting_16) / sizeof(s16);
        while (i--)
        {
            if ((*(u16 *)data) == interesting_16[i])
                return;
        }
    }

    if (len == 4)
    {
        i = sizeof(interesting_32) / sizeof(s32);
        while (i--)
        {
            if ((*(u32 *)data) == interesting_32[i])
                return;
        }
    }

    if (pointedToken.count(token) != 0)
        return;

    if (autoToken.count(token) == 0)
    {
        autoToken[token] = 1;
    }
    else
    {
        autoToken[token]++;
    }
}

static u8 BitFlip(char **argv, u64 len, u8 *filemap, u8 flipnum, u8 flipstep,
                  seed_container::iterator qCur)
{
    u64 i;
    u64 stageCur;
    if (len << 3 < flipnum - flipstep)
        return 0;
    u64 stageMax = (len << 3) - (flipnum - flipstep);
    u32 prevcksum = qCur->cksum;
    string token;
    token.clear();

    for (stageCur = 0; stageCur < stageMax; stageCur += flipstep)
    {
        if (flipnum != 8 && flipstep == 8)
        {
            u32 step = flipnum / flipstep;
            bool useful_flag = false;
            for (i = 0; i < step; i++)
            {
                if (useful_byte[(stageCur >> 3) + i])
                {
                    useful_flag = true;
                    break;
                }
            }
            if (useful_flag == false)
            {
                continue;
            }
        }

        // most time is spent on bitflip(1/1 2/1 4/1)
        // cout << (int)filemap[stageCur >> 3] << endl;
        for (i = 0; i < flipnum; i++)
            FLIP_BIT(filemap, stageCur + i);
        // cout << (int)filemap[stageCur >> 3] << endl;
        if (FuzzUnit(argv, filemap, len))
            return 1;
        for (i = 0; i < flipnum; i++)
            FLIP_BIT(filemap, stageCur + i);

        if (flipnum == 1 && flipstep == 1)
        {
            if (stageCur % 8 == 0)
            {
                u32 cksum = hash32(globalTraceBit, MAP_SIZE, HASH_CONST);
                if (cksum != prevcksum)
                {
                    if (token.size() >= MIN_TOKEN_SIZE &&
                        token.size() <= MAX_TOKEN_SIZE)
                    {
                        AddToken(token);
                    }
                    token.clear();
                    prevcksum = cksum;
                }
                else if (stageCur == stageMax - 1)
                {
                    if (token.size() < MAX_TOKEN_SIZE)
                        token += (char)filemap[stageCur >> 3];

                    if (token.size() >= MIN_TOKEN_SIZE)
                    {
                        AddToken(token);
                    }
                }

                if (cksum != qCur->cksum)
                {
                    if (token.size() < MAX_TOKEN_SIZE)
                        token += (char)filemap[stageCur >> 3];
                }
            }
        }
        else if (flipnum == 8 && flipstep == 8)
        {
            if (!useful_byte[stageCur >> 3])
            {
                u32 cksum = hash32(globalTraceBit, MAP_SIZE, HASH_CONST);
                if (cksum != qCur->cksum)
                {
                    useful_byte[stageCur >> 3] = true;
                }
            }
        }
    }

    print_result("BitFlip-" + to_string(flipnum) + "-" + to_string(flipstep));
    return 0;
}

static u8 Arithmetic8(char **argv, u64 len, u8 *filemap)
{
    u32 i, j;
    for (i = 0; i < len; i++)
    {
        if (useful_byte[i] == false)
        {
            continue;
        }

        u8 orig = filemap[i];
        for (j = 1; j <= ARITH_MAX; j++)
        {
            u8 r = orig ^ (orig + j);
            if (!could_be_bitflip(r))
            {
                filemap[i] = orig + j;
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }

            r = orig ^ (orig - j);
            if (!could_be_bitflip(r))
            {
                filemap[i] = orig - j;
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }
        }
        filemap[i] = orig;
    }
    print_result("Arithmetic8");
    return 0;
}

static u8 Arithmetic16(char **argv, u64 len, u8 *filemap)
{
    u64 i, j;
    if (len < 2)
        return 0;
    for (i = 0; i < len - 1; i++)
    {
        if (useful_byte[i] == false && useful_byte[i + 1] == false)
        {
            continue;
        }

        u16 orig = *(u16 *)(filemap + i);
        for (j = 1; j <= ARITH_MAX; j++)
        {
            u16 r1 = orig ^ (orig + j);
            u16 r2 = orig ^ (orig - j);
            u16 r3 = orig ^ SWAP16(SWAP16(orig) + j);
            u16 r4 = orig ^ SWAP16(SWAP16(orig) - j);

            if ((orig & 0xff) + j > 0xff && !could_be_bitflip(r1))
            {
                *(u16 *)(filemap + i) = orig + j;
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }

            if ((orig & 0xff) < j && !could_be_bitflip(r2))
            {
                *(u16 *)(filemap + i) = orig - j;
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }

            if ((orig >> 8) + j > 0xff && !could_be_bitflip(r3))
            {
                *(u16 *)(filemap + i) = SWAP16(SWAP16(orig) + j);
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }

            if ((orig >> 8) < j && !could_be_bitflip(r4))
            {
                *(u16 *)(filemap + i) = SWAP16(SWAP16(orig) - j);
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }
        }
        *(u16 *)(filemap + i) = orig;
    }
    print_result("Arithmetic16");
    return 0;
}

static u8 Arithmetic32(char **argv, u64 len, u8 *filemap)
{
    u64 i, j;
    if (len < 4)
        return 0;
    for (i = 0; i < len - 3; i++)
    {
        if (useful_byte[i] == false && useful_byte[i + 1] == false &&
            useful_byte[i + 2] == false && useful_byte[i + 3] == false)
        {
            continue;
        }

        u32 orig = *(u32 *)(filemap + i);
        for (j = 1; j <= ARITH_MAX; j++)
        {
            u32 r1 = orig ^ (orig + j);
            u32 r2 = orig ^ (orig - j);
            u32 r3 = orig ^ SWAP32(SWAP32(orig) + j);
            u32 r4 = orig ^ SWAP32(SWAP32(orig) - j);

            if ((orig & 0xffff) + j > 0xffff && !could_be_bitflip(r1))
            {
                *(u32 *)(filemap + i) = orig + j;
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }

            if ((orig & 0xffff) < j && !could_be_bitflip(r2))
            {
                *(u32 *)(filemap + i) = orig - j;
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }

            if ((orig >> 16) + j > 0xff && !could_be_bitflip(r3))
            {
                *(u32 *)(filemap + i) = SWAP32(SWAP32(orig) + j);
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }

            if ((orig >> 16) < j && !could_be_bitflip(r4))
            {
                *(u32 *)(filemap + i) = SWAP32(SWAP32(orig) - j);
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }
        }
        *(u32 *)(filemap + i) = orig;
    }
    print_result("Arithmetic32");
    return 0;
}

static u8 InterestingReplace8(char **argv, u64 len, u8 *filemap)
{
    u64 i, j;
    for (i = 0; i < len; i++)
    {
        if (useful_byte[i] == false)
        {
            continue;
        }

        u8 orig = filemap[i];
        for (j = 0; j < sizeof(interesting_8) / sizeof(s8); j++)
        {
            if (!could_be_bitflip(orig ^ (u8)interesting_8[j]) &&
                !could_be_arith(orig, (u8)interesting_8[j], 1))
            {

                filemap[i] = interesting_8[j];
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }
        }
        filemap[i] = orig;
    }
    print_result("InterestingReplace8");
    return 0;
}

static u8 InterestingReplace16(char **argv, u64 len, u8 *filemap)
{
    u64 i, j;
    if (len < 2)
        return 0;
    for (i = 0; i < len - 1; i++)
    {
        if (useful_byte[i] == false && useful_byte[i + 1] == false)
        {
            continue;
        }

        u16 orig = *(u16 *)(filemap + i);
        for (j = 0; j < sizeof(interesting_16) / sizeof(s16); j++)
        {
            if (!could_be_bitflip(orig ^ (u16)interesting_16[j]) &&
                !could_be_arith(orig, (u16)interesting_16[j], 2) &&
                !could_be_interest(orig, (u16)interesting_16[j], 2, 0))
            {

                *(u16 *)(filemap + i) = interesting_16[j];
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }
        }
        *(u16 *)(filemap + i) = orig;
    }
    print_result("InterestingReplace16");
    return 0;
}

static u8 InterestingReplace32(char **argv, u64 len, u8 *filemap)
{
    u64 i, j;
    if (len < 4)
        return 0;
    for (i = 0; i < len - 3; i++)
    {
        if (useful_byte[i] == false && useful_byte[i + 1] == false &&
            useful_byte[i + 2] == false && useful_byte[i + 3] == false)
        {
            continue;
        }

        u32 orig = *(u32 *)(filemap + i);
        for (j = 0; j < sizeof(interesting_32) / sizeof(s32); j++)
        {
            if (!could_be_bitflip(orig ^ (u32)interesting_32[j]) &&
                !could_be_arith(orig, (u32)interesting_32[j], 4) &&
                !could_be_interest(orig, (u32)interesting_32[j], 4, 0))
            {

                *(u32 *)(filemap + i) = interesting_32[j];
                if (FuzzUnit(argv, filemap, len))
                    return 1;
            }
        }
        *(u32 *)(filemap + i) = orig;
    }
    print_result("InterestingReplace32");
    return 0;
}

static u8 DictionaryReplace(char **argv, u64 len, u8 *filemap,
                            vector<pair<string, u32>> &MVector)
{
    u64 i, j;
    u8 *token_data;
    u32 token_len;
    bool should;
    u8 orig[MAX_TOKEN_SIZE];

    for (i = 0; i < len; i++)
    {
        memcpy(orig, filemap + i, min(MAX_TOKEN_SIZE, (int)(len - i)));
        // the tokens are sorted from short string to long string
        for (auto &token : MVector)
        {
            token_data = (u8 *)token.first.c_str();
            token_len = token.first.size();

            if (token_len > len - i || !memcmp(token_data, orig, token_len))
            {
                continue;
            }

            should = false;
            for (j = i; j < i + token_len; j++)
            {
                if (useful_byte[j] == true)
                {
                    should = true;
                    break;
                }
            }
            if (should == false)
                continue;

            memcpy(filemap + i, token_data, token_len);
            if (FuzzUnit(argv, filemap, len))
                return 1;
        }
        memcpy(filemap + i, orig, min(MAX_TOKEN_SIZE, (int)(len - i)));
    }
    print_result("DictionaryReplace");
    return 0;
}

static u8 DictionaryInsert(char **argv, u64 len, u8 *filemap,
                           const vector<pair<string, u32>> &MVector)
{
    u64 i, j;
    u8 *token_data;
    u32 token_len;
    unique_ptr<u8[]> temp(new u8[len + MAX_TOKEN_SIZE]);
    u8 *temp_ptr = temp.get();

    // u8 *temp = new u8[len + MAX_TOKEN_SIZE];
    for (i = 0; i < len; i++)
    {
        // the tokens are sorted from short string to long string
        for (auto &token : MVector)
        {
            token_data = (u8 *)token.first.c_str();
            token_len = token.first.size();

            if (len + token_len > MAX_FILE_LEN)
                continue;

            memcpy(temp_ptr + i, token_data, token_len);
            memcpy(temp_ptr + i + token_len, filemap + i, len - i);
            if (FuzzUnit(argv, temp_ptr, len + token_len))
            {
                return 1;
            }
        }
        temp_ptr[i] = filemap[i];
    }
    print_result("DictionaryInsert");
    return 0;
}

bool CompareMVectorValue(const pair<string, u32> &obj1,
                         const pair<string, u32> &obj2)
{
    return obj1.second > obj2.second; // sort from big value to small value
}

bool CompareMVectorStringLen(const pair<string, u32> &obj1,
                             const pair<string, u32> &obj2)
{
    return obj1.first.size() <
           obj2.first.size(); // sort from small size to big size
}

static u8 Dictionary(char **argv, u64 len, u8 *filemap)
{
    vector<pair<string, u32>> MVector(autoToken.begin(), autoToken.end());
    if (MVector.size() > MAX_TOKEN_NUM)
    {
        sort(MVector.begin(), MVector.end(), CompareMVectorValue);
        MVector.resize(MAX_TOKEN_NUM);
    }
    sort(MVector.begin(), MVector.end(), CompareMVectorStringLen);

    if (DictionaryReplace(argv, len, filemap, MVector))
    {
        cout << "DictionaryReplace() fail" << endl;
        return 1;
    }

    if (DictionaryInsert(argv, len, filemap, MVector))
    {
        cout << "DictionaryInsert() fail" << endl;
        return 1;
    }

    return 0;
}

static u32 choose_block_len(u32 limit)
{
    u32 min_value, max_value;
    u32 rlim = min(queueCycle, (u64)3);

    switch (randNum(rlim))
    {
    case 0:
        min_value = 1;
        max_value = HAVOC_BLK_SMALL;
        break;

    case 1:
        min_value = HAVOC_BLK_SMALL;
        max_value = HAVOC_BLK_MEDIUM;
        break;

    default:

        if (randNum(10))
        {
            min_value = HAVOC_BLK_MEDIUM;
            max_value = HAVOC_BLK_LARGE;
        }
        else
        {
            min_value = HAVOC_BLK_LARGE;
            max_value = HAVOC_BLK_XL;
        }
    }
    if (min_value >= limit)
        min_value = 1;
    return min_value + randNum(min(max_value, limit) - min_value + 1);
}

static u8 Havoc(char **argv, u64 len, unique_ptr<u8[]> &uniq_ptr,
                u32 cycles = HAVOC_CYCLES_INIT)
{
    u8 *filemap = uniq_ptr.get();
    u64 temp_len = len;
    u32 i, j;
    u32 queue_init_size = queue.size();
    u32 queue_havoc_size;
    // TODO: should be different in different queue
    u32 havoc_time = cycles * HAVOC_INIT_MULT;

    unique_ptr<u8[]> orig_buf(new u8[len]);
    memcpy(orig_buf.get(), filemap, len);

    for (i = 0; i < havoc_time; i++)
    {
        u32 mutate_time = 1 << (1 + randNum(HAVOC_STACK_POW2));
        for (j = 0; j < mutate_time; j++)
        {
            switch (randNum(15 +
                            ((pointedToken.size() + autoToken.size()) ? 2 : 0)))
            {
            case 0:
                // random bit flip
                FLIP_BIT(filemap, randNum(temp_len << 3));
                break;

            case 1:
                // random byte replace
                filemap[randNum(temp_len)] =
                    interesting_8[randNum(sizeof(interesting_8))];
                break;

            case 2:
                // random word replace
                if (temp_len < 2)
                    break;
                if (randNum(2))
                {
                    *(u16 *)(filemap + randNum(temp_len - 1)) =
                        interesting_16[randNum(sizeof(interesting_16) >> 1)];
                }
                else
                {
                    *(u16 *)(filemap + randNum(temp_len - 1)) = SWAP16(
                        interesting_16[randNum(sizeof(interesting_16) >> 1)]);
                }
                break;

            case 3:
                /* Set dword to interesting value, randomly choosing endian. */
                if (temp_len < 4)
                    break;
                if (randNum(2))
                {
                    *(u32 *)(filemap + randNum(temp_len - 3)) =
                        interesting_32[randNum(sizeof(interesting_32) >> 2)];
                }
                else
                {
                    *(u32 *)(filemap + randNum(temp_len - 3)) = SWAP32(
                        interesting_32[randNum(sizeof(interesting_32) >> 2)]);
                }
                break;

            case 4:
                /* Randomly subtract from byte. */
                filemap[randNum(temp_len)] -= 1 + randNum(ARITH_MAX);
                break;

            case 5:
                /* Randomly add to byte. */
                filemap[randNum(temp_len)] += 1 + randNum(ARITH_MAX);
                break;

            case 6:
                /* Randomly subtract from word, random endian. */
                if (temp_len < 2)
                    break;
                if (randNum(2))
                {
                    u32 pos = randNum(temp_len - 1);
                    *(u16 *)(filemap + pos) -= 1 + randNum(ARITH_MAX);
                }
                else
                {
                    u32 pos = randNum(temp_len - 1);
                    u16 num = 1 + randNum(ARITH_MAX);

                    *(u16 *)(filemap + pos) =
                        SWAP16(SWAP16(*(u16 *)(filemap + pos)) - num);
                }
                break;

            case 7:
                /* Randomly add to word, random endian. */
                if (temp_len < 2)
                    break;
                if (randNum(2))
                {
                    u32 pos = randNum(temp_len - 1);
                    *(u16 *)(filemap + pos) += 1 + randNum(ARITH_MAX);
                }
                else
                {
                    u32 pos = randNum(temp_len - 1);
                    u16 num = 1 + randNum(ARITH_MAX);

                    *(u16 *)(filemap + pos) =
                        SWAP16(SWAP16(*(u16 *)(filemap + pos)) + num);
                }
                break;

            case 8:
                /* Randomly subtract from dword, random endian. */
                if (temp_len < 4)
                    break;
                if (randNum(2))
                {
                    u32 pos = randNum(temp_len - 3);

                    *(u32 *)(filemap + pos) -= 1 + randNum(ARITH_MAX);
                }
                else
                {
                    u32 pos = randNum(temp_len - 3);
                    u32 num = 1 + randNum(ARITH_MAX);

                    *(u32 *)(filemap + pos) =
                        SWAP32(SWAP32(*(u32 *)(filemap + pos)) - num);
                }

                break;

            case 9:
                /* Randomly add to dword, random endian. */
                if (temp_len < 4)
                    break;
                if (randNum(2))
                {
                    u32 pos = randNum(temp_len - 3);

                    *(u32 *)(filemap + pos) += 1 + randNum(ARITH_MAX);
                }
                else
                {
                    u32 pos = randNum(temp_len - 3);
                    u32 num = 1 + randNum(ARITH_MAX);

                    *(u32 *)(filemap + pos) =
                        SWAP32(SWAP32(*(u32 *)(filemap + pos)) + num);
                }

                break;

            case 10:
                // set a random byte to a random value
                filemap[randNum(temp_len)] ^= 1 + randNum(255);
                break;

            case 11 ... 12:
            {
                u32 del_from, del_len;
                if (temp_len < 2)
                    break;
                del_len = choose_block_len(temp_len - 1);
                del_from = randNum(temp_len - del_len + 1);
                memmove(filemap + del_from, filemap + del_from + del_len,
                        temp_len - del_from - del_len);

                temp_len -= del_len;
                break;
            }

            case 13:
                if (temp_len + HAVOC_BLK_XL < MAX_FILE_LEN)
                {
                    u8 actually_clone = randNum(4);
                    u32 clone_from, clone_to, clone_len;
                    u8 *new_buf;

                    if (actually_clone)
                    {
                        clone_len = choose_block_len(temp_len);
                        clone_from = randNum(temp_len - clone_len + 1);
                    }
                    else
                    {
                        clone_len = choose_block_len(HAVOC_BLK_XL);
                        clone_from = 0;
                    }

                    clone_to = randNum(temp_len);
                    new_buf = new u8[temp_len + clone_len];

                    // Head
                    memcpy(new_buf, filemap, clone_to);

                    // Inserted part
                    if (actually_clone)
                        memcpy(new_buf + clone_to, filemap + clone_from,
                               clone_len);
                    else
                        memset(new_buf + clone_to,
                               randNum(2) ? randNum(256)
                                          : filemap[randNum(temp_len)],
                               clone_len);

                    // Tail
                    memcpy(new_buf + clone_to + clone_len, filemap + clone_to,
                           temp_len - clone_to);

                    uniq_ptr.reset(new_buf);
                    filemap = uniq_ptr.get();
                    temp_len += clone_len;
                }
                break;

            case 14:
            {
                u32 copy_from, copy_to, copy_len;
                if (temp_len < 2)
                    break;

                copy_len = choose_block_len(temp_len - 1);
                copy_from = randNum(temp_len - copy_len + 1);
                copy_to = randNum(temp_len - copy_len + 1);

                if (randNum(4))
                {
                    if (copy_from != copy_to)
                        memmove(filemap + copy_to, filemap + copy_from,
                                copy_len);
                }
                else
                    memset(filemap + copy_to,
                           randNum(2) ? randNum(256)
                                      : filemap[randNum(temp_len)],
                           copy_len);
                break;
            }

                /* Values 15 and 16 can be selected only if there are any extras
                   present in the dictionaries. */

            case 15:
            {
                /* Overwrite bytes with an extra. */
                if (!pointedToken.size() || (autoToken.size() && randNum(2)))
                {
                    /* No user-specified extras or odds in our favor. Let's use
                       an auto-detected one. */

                    u32 use_extra = randNum(autoToken.size());
                    auto iter = autoToken.begin();
                    advance(iter, use_extra);
                    u32 extra_len = iter->first.size();
                    u32 insert_at;

                    if (extra_len > temp_len)
                        break;

                    insert_at = randNum(temp_len - extra_len + 1);
                    memcpy(filemap + insert_at, iter->first.c_str(), extra_len);
                }
                else
                {
                    /* No auto extras or odds in our favor. Use the dictionary.
                     */
                    u32 use_extra = randNum(pointedToken.size());
                    auto iter = pointedToken.begin();
                    advance(iter, use_extra);
                    u32 extra_len = iter->first.size();
                    u32 insert_at;

                    if (extra_len > temp_len)
                        break;

                    insert_at = randNum(temp_len - extra_len + 1);
                    memcpy(filemap + insert_at, iter->first.c_str(), extra_len);
                }

                break;
            }

            case 16:
            {
                u32 use_extra, extra_len, insert_at = randNum(temp_len + 1);
                u8 *new_buf;

                /* Insert an extra. Do the same dice-rolling stuff as for the
                     previous case. */

                if (!pointedToken.size() || (autoToken.size() && randNum(2)))
                {
                    use_extra = randNum(autoToken.size());
                    auto iter = autoToken.begin();
                    advance(iter, use_extra);
                    extra_len = iter->first.size();

                    if (temp_len + extra_len >= MAX_FILE_LEN)
                        break;

                    new_buf = new u8[temp_len + extra_len];
                    /* Head */
                    memcpy(new_buf, filemap, insert_at);

                    /* Inserted part */
                    memcpy(new_buf + insert_at, iter->first.c_str(), extra_len);
                }
                else
                {
                    use_extra = randNum(pointedToken.size());
                    auto iter = pointedToken.begin();
                    advance(iter, use_extra);
                    extra_len = iter->first.size();

                    if (temp_len + extra_len >= MAX_FILE_LEN)
                        break;

                    new_buf = new u8[temp_len + extra_len];

                    /* Head */
                    memcpy(new_buf, filemap, insert_at);

                    /* Inserted part */
                    memcpy(new_buf + insert_at, iter->first.c_str(), extra_len);
                }

                /* Tail */
                memcpy(new_buf + insert_at + extra_len, filemap + insert_at,
                       temp_len - insert_at);

                uniq_ptr.reset(new_buf);
                filemap = uniq_ptr.get();
                temp_len += extra_len;

                break;
            }
            }
        }
        if (FuzzUnit(argv, filemap, temp_len))
            return 1;

        if (temp_len < len)
        {
            u8 *new_buf = new u8[len];
            uniq_ptr.reset(new_buf);
            filemap = uniq_ptr.get();
        }
        memcpy(filemap, orig_buf.get(), len);
        temp_len = len;

        // TODO: change if some interesting queue found
        queue_havoc_size = queue.size();
        if (queue_havoc_size > queue_init_size)
        {
            if (havoc_time < HAVOC_CYCLES_INIT * HAVOC_MAX_MULT)
            {
                havoc_time *= 2;
            }
        }
        queue_init_size = queue_havoc_size;
    }
    print_result("Havoc");
    return 0;
}

static void locate_diffs(u8 *ptr1, u8 *ptr2, u32 len, s32 &first, s32 &last)
{
    s32 f_loc = -1;
    s32 l_loc = -1;
    u32 pos;

    for (pos = 0; pos < len; pos++)
    {
        if (*(ptr1++) != *(ptr2++))
        {
            if (f_loc == -1)
                f_loc = pos;
            l_loc = pos;
        }
    }

    first = f_loc;
    last = l_loc;

    return;
}

static u8 Splice(char **argv, u64 len, unique_ptr<u8[]> &uniq_ptr,
                 seed_container::iterator qCur)
{
    u32 other, splice_cycles = 0;
    u32 splice_at;
    s32 fd, res, f_diff, l_diff;
    seed_container::iterator other_que;
    if (queue.size() <= 1 || len <= 1)
        return 0;

    while (splice_cycles++ < SPLICE_CYCLES)
    {
        unique_ptr<u8[]> other_buf;
        do
        {
            other = randNum(queue.size());
            other_que = queue.begin();
            advance(other_que, other);
        } while (other_que == qCur);

        while (other_que != queue.end() && other_que->fileLen < 2)
        {
            other_que++;
        }

        if (other_que == queue.end())
            continue;

        fd = open(other_que->fileName.c_str(), O_RDONLY);
        if (fd < 0)
        {
            cout << "open " << other_que->fileName << " fail in Splice" << endl;
            return 1;
        }

        other_buf = unique_ptr<u8[]>(new u8[other_que->fileLen]);
        res = read(fd, other_buf.get(), other_que->fileLen);
        if (res != other_que->fileLen)
        {
            cout << "read " << other_que->fileName << " fail in Splice" << endl;
            return 1;
        }

        close(fd);

        locate_diffs(uniq_ptr.get(), other_buf.get(),
                     min(len, other_que->fileLen), f_diff, l_diff);
        if (f_diff < 0 || l_diff < 2 || f_diff == l_diff)
        {
            continue;
        }

        splice_at = f_diff + randNum(l_diff - f_diff);
        // len = other_que->fileLen;
        memcpy(other_buf.get(), uniq_ptr.get(), splice_at);

        if (Havoc(argv, other_que->fileLen, other_buf, SPLICE_HAVOC))
        {
            cout << "Fail to Havoc in Splice" << endl;
            return 1;
        }
    }
    print_result("Splice");
    return 0;
}

string GetTimeStr(void)
{
    u64 timeNow = GetCurTimeUs();
    u64 timeDiff = timeNow - timeBegin;

    u64 second_num = timeDiff / 1000000ULL;
    u64 minute_num = second_num / 60;

    u64 hours = minute_num / 60;
    u64 minutes = minute_num % 60;
    u64 seconds = second_num % 60;

    std::stringstream ss;
    ss << hours << "h " << minutes << "m " << seconds << "s";
    return ss.str();
}

void ShowTime(void) { cout << "Fuzzing has lasted " << GetTimeStr() << endl; }

u8 FuzzMain(seed_container::iterator qCur, char **argv)
{
    // cout << "--------------------" << endl;
    // cout << "Fuzzing for New seed" << endl;
    u64 time_start, time_end;
    u32 tmp_coverage;
    u64 excv_before;

    // cout << "Check if the seed should be fuzzed" << endl;
    if (pendFavor)
    {
        if ((qCur->wasFuzzed || !qCur->favored) &&
            randNum(100) < SKIP_TO_NEW_PROB)
        {
            return FUZZING_SKIP;
        }
    }
    else if (!qCur->favored)
    {
        if (queueCycle > 1 && !qCur->wasFuzzed)
        {
            if (randNum(100) < SKIP_NFAV_NEW_PROB)
                return FUZZING_SKIP;
        }
        else
        {
            if (randNum(100) < SKIP_NFAV_OLD_PROB)
                return FUZZING_SKIP;
        }
    }

    // get test file content
    cout << "Get the file content of seed" << endl;
    s32 fd = open(qCur->fileName.c_str(), O_RDONLY);
    if (fd < 0)
    {
        cout << "Fail to open the file: " << qCur->fileName << endl;
        exit(-1);
    }

    u64 len = qCur->fileLen;
    u8 *inBuf = (u8 *)mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (inBuf == MAP_FAILED)
    {
        cout << "Fail to mmap the file: " << qCur->fileName << endl;
        exit(-1);
    }
    close(fd);

    unique_ptr<u8[]> outBuf(new u8[len]);
    u8 *outBuf_ptr = outBuf.get();

#ifndef NO_FUZZING_ERROR
    unique_ptr<u8[]> unique_error_list(new u8[MAX_LIST_SIZE]);
    u8 *error_list = unique_error_list.get();
    if (qCur->elistFileName.empty())
    {
        memset(error_list, 0, MAX_LIST_SIZE * sizeof(u8));
    }
    else
    {
        ReadFile2Mem(qCur->elistFileName, error_list, MAX_LIST_SIZE);
    }

    // When trimming, the testcase is fault injected.
    memcpy(errorList, error_list, MAX_LIST_SIZE * sizeof(u8));
#endif

#ifndef NO_CONCURRENCY_FUZZ
    unique_ptr<matrix_element[]> unique_control_matrix(
        new matrix_element[CONTROL_MATRIX_SIZE]);
    auto tmp_control_matrix = unique_control_matrix.get();
    if (qCur->control_matrix_file.empty())
    {
        memset(tmp_control_matrix, 0,
               CONTROL_MATRIX_SIZE * sizeof(matrix_element));
    }
    else
    {
        ReadFile2Mem(qCur->control_matrix_file, (u8 *)tmp_control_matrix,
                     CONTROL_MATRIX_SIZE * sizeof(matrix_element));
    }
    memcpy(control_matrix, tmp_control_matrix,
           CONTROL_MATRIX_SIZE * sizeof(matrix_element));
#endif

    // u8 *outBuf = new u8[len];

    // calibration-----useless
#ifndef NO_FUZZING_FILE
    // triming
    // cout << "Trimming the file" << endl;
    // if (!qCur->trimDone) {
    //     u8 res = TrimCase(argv, qCur, inBuf);
    //     if (res == FAULT_ERROR) {
    //         cout << "TrimCase() fail int FuzzOne" << endl;
    //         exit(-1);
    //     }
    //     qCur->trimDone = true;
    //     if (len != qCur->fileLen) len = qCur->fileLen;
    // }
#endif
    memcpy(outBuf_ptr, inBuf, len);

    unique_ptr<bool[]> unique_useful_byte(new bool[len]);
    useful_byte = unique_useful_byte.get();

    if (len < MIN_FILE_LEN_FOR_USEFUL)
    {
        memset(useful_byte, true, sizeof(bool) * len);
    }
    else
    {
        memset(useful_byte, false, sizeof(bool) * len);
    }

    u32 queue_found = queue.size();

#ifndef NO_FUZZING_ERROR
    cout << "\nEnter Stage: Fault Injection" << endl;
    excv_before = exec_count;
    time_start = GetCurTimeUs();
    if (ErrorListMutate(argv, len, outBuf_ptr, error_list))
    {
        cout << "Fail in fault injection" << endl;
        munmap(inBuf, len);
        return FUZZING_ERROR;
    }
    time_end = GetCurTimeUs();
    cout << "Fault Injection find " << queue.size() - queue_found << endl;
    cout << "average speed = "
         << (exec_count - excv_before) * 1000000 / (time_end - time_start)
         << endl;
    tmp_coverage = ShowCoverage(virginMap);
    cout << "Concurreny coverage:  " << tmp_coverage << " ("
         << 1.0 * tmp_coverage * 100 / MAP_SIZE << "%)" << endl;
    queue_found = queue.size();
    ShowTime();
    // resume the errorList
    memcpy(errorList, error_list, MAX_LIST_SIZE * sizeof(u8));
#endif

#ifndef NO_CONCURRENCY_FUZZ
    cout << "\nEnter Stage: Concurrency fuzzing" << endl;
    excv_before = exec_count;
    time_start = GetCurTimeUs();
    if (control_matrix_mutate(argv, len, outBuf_ptr, tmp_control_matrix))
    {
        cout << "Fail in DelayListMutate()" << endl;
        munmap(inBuf, len);
        return FUZZING_ERROR;
    }
    time_end = GetCurTimeUs();
    cout << "Delay Insertion find: " << queue.size() - queue_found << endl;
    cout << "Average speed: "
         << (exec_count - excv_before) * 1000000 / (time_end - time_start)
         << endl;
    tmp_coverage = ShowCoverage(virginMap);
    cout << "Traditional coverage: " << tmp_coverage << " ("
         << 1.0 * tmp_coverage * 100 / MAP_SIZE << "%)" << endl;
    queue_found = queue.size();
    ShowTime();
    // resume the errorList
    memcpy(control_matrix, tmp_control_matrix, MAX_LIST_SIZE * sizeof(u32));
#endif
    // TODO:performance score

#ifndef NO_FUZZING_FILE
    if (!skip_deterministic)
    {
        // Bit flip
        cout << "\nEnter Stage: Bit Flip" << endl;
        excv_before = exec_count;
        time_start = GetCurTimeUs();
        if (BitFlip(argv, len, outBuf_ptr, 1, 1, qCur) ||
            BitFlip(argv, len, outBuf_ptr, 2, 1, qCur) ||
            BitFlip(argv, len, outBuf_ptr, 4, 1, qCur) ||
            BitFlip(argv, len, outBuf_ptr, 8, 8, qCur) ||
            BitFlip(argv, len, outBuf_ptr, 16, 8, qCur) ||
            BitFlip(argv, len, outBuf_ptr, 32, 8, qCur))
        {
            cout << "Fail to bit flip" << endl;
            munmap(inBuf, len);
            return FUZZING_ERROR;
        }
        time_end = GetCurTimeUs();
        cout << "Bit Flip find " << queue.size() - queue_found << endl;
        cout << "average speed = "
             << (exec_count - excv_before) * 1000000 / (time_end - time_start)
             << endl;
        tmp_coverage = ShowCoverage(virginMap);
        cout << "Traditional coverage: " << tmp_coverage << " ("
             << 1.0 * tmp_coverage * 100 / MAP_SIZE << "%)" << endl;
        queue_found = queue.size();
        ShowTime();

        // for (u32 i = 0; i < len; i++) {
        //     cout << "useful_byte[" << i << "] = " << useful_byte[i] << endl;
        // }

        // Arithmetic
        cout << "\nEnter Stage: Arithmetic" << endl;
        excv_before = exec_count;
        time_start = GetCurTimeUs();
        if (Arithmetic8(argv, len, outBuf_ptr) ||
            Arithmetic16(argv, len, outBuf_ptr) ||
            Arithmetic32(argv, len, outBuf_ptr))
        {
            cout << "Fail to arithmetic" << endl;
            munmap(inBuf, len);
            return FUZZING_ERROR;
        }
        time_end = GetCurTimeUs();
        cout << "Arithmetic find " << queue.size() - queue_found << endl;
        cout << "average speed = "
             << (exec_count - excv_before) * 1000000 / (time_end - time_start)
             << endl;
        tmp_coverage = ShowCoverage(virginMap);
        cout << "Traditional coverage: " << tmp_coverage << " ("
             << 1.0 * tmp_coverage * 100 / MAP_SIZE << "%)" << endl;
        queue_found = queue.size();
        ShowTime();

        // Interesting value
        cout << "\nEnter Stage: InterestingReplace" << endl;
        excv_before = exec_count;
        time_start = GetCurTimeUs();
        if (InterestingReplace8(argv, len, outBuf_ptr) ||
            InterestingReplace16(argv, len, outBuf_ptr) ||
            InterestingReplace32(argv, len, outBuf_ptr))
        {
            cout << "Fail to interesting replace" << endl;
            munmap(inBuf, len);
            return FUZZING_ERROR;
        }
        time_end = GetCurTimeUs();
        cout << "Interesting value find " << queue.size() - queue_found << endl;
        cout << "average speed = "
             << (exec_count - excv_before) * 1000000 / (time_end - time_start)
             << endl;
        tmp_coverage = ShowCoverage(virginMap);
        cout << "Traditional coverage: " << tmp_coverage << " ("
             << 1.0 * tmp_coverage * 100 / MAP_SIZE << "%)" << endl;
        queue_found = queue.size();
        ShowTime();

        // Dictionary
        cout << "\nEnter Stage: Dictionary" << endl;
        excv_before = exec_count;
        time_start = GetCurTimeUs();
        if (Dictionary(argv, len, outBuf.get()))
        {
            munmap(inBuf, len);
            return FUZZING_ERROR;
        }
        time_end = GetCurTimeUs();
        cout << "Dictionary find " << queue.size() - queue_found << endl;
        cout << "average speed = "
             << (exec_count - excv_before) * 1000000 / (time_end - time_start)
             << endl;
        tmp_coverage = ShowCoverage(virginMap);
        cout << "Traditional coverage: " << tmp_coverage << " ("
             << 1.0 * tmp_coverage * 100 / MAP_SIZE << "%)" << endl;
        queue_found = queue.size();
        ShowTime();
    }
    // Havoc
    cout << "\nEnter Stage: Havoc" << endl;
    excv_before = exec_count;
    time_start = GetCurTimeUs();
    if (Havoc(argv, len, outBuf))
    {
        cout << "Fail to Havoc" << endl;
        munmap(inBuf, len);
        return FUZZING_ERROR;
    }
    time_end = GetCurTimeUs();
    cout << "Havoc find " << queue.size() - queue_found << endl;
    cout << "average speed = "
         << (exec_count - excv_before) * 1000000 / (time_end - time_start)
         << endl;
    tmp_coverage = ShowCoverage(virginMap);
    cout << "Traditional coverage: " << tmp_coverage << " ("
         << 1.0 * tmp_coverage * 100 / MAP_SIZE << "%)" << endl;
    queue_found = queue.size();
    ShowTime();

    // Splice
    cout << "\nEnter Stage: Splice" << endl;
    excv_before = exec_count;
    time_start = GetCurTimeUs();
    if (Splice(argv, len, outBuf, qCur))
    {
        cout << "Fail to Splice" << endl;
        munmap(inBuf, len);
        return FUZZING_ERROR;
    }
    time_end = GetCurTimeUs();
    cout << "Splice find " << queue.size() - queue_found << endl;
    // Don't try to calculate the average speed of Splice, because (time_end -
    // time_start) may be too little (equal to zero) cout << "average speed = "
    // << (exec_count - excv_before) * 1000000 / (time_end - time_start) <<
    // endl;
    tmp_coverage = ShowCoverage(virginMap);
    cout << "Traditional coverage: " << tmp_coverage << " ("
         << 1.0 * tmp_coverage * 100 / MAP_SIZE << "%)" << endl;
    queue_found = queue.size();
    ShowTime();
#endif

    qCur->wasFuzzed = true;
    munmap(inBuf, qCur->fileLen);

    cout << "queue.size = " << queue.size() << endl;
    return 0;
}
