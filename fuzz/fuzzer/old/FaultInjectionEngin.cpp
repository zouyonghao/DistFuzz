#include "Fuzzer.h"
#include <set>
#define ERROR_LIST_SIZE_BYTE 4

typedef set<u32> fault_unit;
typedef list<fault_unit> fault_unit_list;

static u8 tmp_trace[MAX_LIST_SIZE];

static fault_unit_list fu_list; // save the crashed fault_unit

inline u8 show_bit(u8 *mem, u32 bit_loc)
{
    return mem[bit_loc];
    // return mem[bit_loc >> 3] & (128 >> (bit_loc & 7));
}

inline void flip_bit(u8 *mem, u32 bit_loc)
{
    // mem[bit_loc >> 3] ^= (128 >> (bit_loc & 7));
    mem[bit_loc] = 1 - mem[bit_loc];
}

bool is_unique_fault(const u8 *const error_list)
{
    fault_unit fu;
    u32 i;
    bool flag;
    for (i = 0; i < MAX_LIST_SIZE; i++)
    {
        if (error_list[i] == 1)
        {
            fu.insert(i);
        }
    }

    for (auto &fu_test : fu_list)
    {
        // check whether the fu_test is the subset of fu
        flag = true;
        for (auto &num : fu_test)
        {
            if (!fu.count(num))
            {
                flag = false;
                break;
            }
        }
        // if fu_test is the subset, fu is not unique
        if (flag == true)
            return false;
    }

    // the fu is not the super set of any fu_test in fu_list
    return true;
}

void save_crashed_fu(const u8 *const error_list)
{
    fault_unit fu;
    u32 i;
    for (i = 0; i < MAX_LIST_SIZE; i++)
    {
        if (error_list[i] == 1)
        {
            fu.insert(i);
        }
    }

    fu_list.push_back(fu);
}

u8 ErrorListMutate(char **argv, u64 len, u8 *filemap, u8 *error_list)
{
    // run once to get errorTrace
    if (FuzzUnit(argv, filemap, len, error_list))
        return 1;
    memcpy(tmp_trace, errorTraced, MAX_LIST_SIZE);

    u32 i;
    // u32 max_error_site = MAX_LIST_SIZE << 3;
    u32 max_error_site = MAX_LIST_SIZE;
    u32 test_time = 0;
    for (i = 0; i < max_error_site; i++)
    {
        if (show_bit(tmp_trace, i) == 0)
        {
            continue;
        }

        // cout << "Injected point: " << i << endl;
        flip_bit(error_list, i);
        if (is_unique_fault(error_list))
        {
            if (FuzzUnit(argv, filemap, len, error_list))
                return 1;
            test_time++;
        }
        flip_bit(error_list, i);
    }
    cout << "test_time = " << test_time << endl;
    print_result("ErrorListMutate");
    return 0;
}