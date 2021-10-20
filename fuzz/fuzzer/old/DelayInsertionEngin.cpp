#include "Fuzzer.h"

#ifndef NO_CONCURRENCY_FUZZ
static matrix_element enable_control_matrix[CONTROL_MATRIX_SIZE];
static matrix_element tmp_adjacency_matrix[ADJACENCY_MATRIX_SIZE];
static matrix_element tmp_travel_matrix[TRAVEL_MATRIX_SIZE];
static matrix_element new_control_matrix[CONTROL_MATRIX_SIZE];
set<u64> unreachable_control_matrix;
int concurrency_test_time = 0;
int skip_since_travel = 0;
int skip_since_unreachable = 0;

u64 hashes2hash(u64 hash_1, u64 hash_2)
{
    u64 hash_sum = hash_1 + hash_2;
    u64 new_hash = 0;
    while (hash_sum != 0)
    {
        new_hash = 131 * new_hash + (hash_sum ^ 15);
        hash_sum = hash_sum >> 4;
    }
    return new_hash;
}

u32 num_of_matrix_elemet(matrix_element *matrix, u32 len)
{
    u32 num = 0;
    for (int i = 0; i < len; i++)
    {
        if (matrix[i].hash_1 == 0 && matrix[i].hash_2 == 0)
            break;
        if (matrix[i].hash_1 != 0 || matrix[i].hash_2 != 0)
            num++;
    }
    return num;
}

bool compare_matrix_element(matrix_element &me_1, matrix_element &me_2)
{
    if (me_1.hash_1 == me_2.hash_1 && me_1.hash_2 == me_2.hash_2)
        return true;
    if (me_1.hash_1 == me_2.hash_2 && me_1.hash_2 == me_2.hash_1)
        return true;
    return false;
}

bool is_control_matrix_triggered(matrix_element *tmp_control_matrix, matrix_element *tmp_travel_matrix)
{
    for (int i = 0; i < CONTROL_MATRIX_SIZE; i++)
    {
        if (tmp_travel_matrix[i].hash_1 == 0 && tmp_control_matrix[i].hash_2 == 0)
            break;

        for (int j = 0; j < TRAVEL_MATRIX_SIZE; j++)
        {
            if (tmp_travel_matrix[j].hash_1 == 0 && tmp_travel_matrix[j].hash_2 == 0)
                break;

            if (compare_matrix_element(tmp_control_matrix[i], tmp_travel_matrix[j]))
            {
                return true;
            }
        }
    }
    return false;
}

u8 has_new_travel_matrix_element(matrix_element *tmp_travel_matrix, set<u64> &tmp_total_travel)
{
    u64 hash1, hash2, hash;
    u8 flag = 0;
    for (int i = 0; i < TRAVEL_MATRIX_SIZE; i++)
    {
        hash1 = tmp_travel_matrix[i].hash_1;
        hash2 = tmp_travel_matrix[i].hash_2;

        if (hash1 == 0 && hash2 == 0)
            break;

        hash = hashes2hash(hash1, hash2);
        if (tmp_total_travel.count(hash) == 0)
        { // has new element
            tmp_total_travel.insert(hash);
            flag = 1;
        }
    }
    return flag;
}

u8 one_element_cm_test(char **argv, u8 *filemap, u64 len, u64 hash_1, u64 hash_2)
{
    u64 hash_total = hashes2hash(hash_1, hash_2);
    if (total_travel.count(hash_total) != 0)
    {
        skip_since_travel++;
        return 0;
    }
    if (unreachable_control_matrix.count(hash_total) != 0)
    {
        skip_since_unreachable++;
        return 0;
    }

    new_control_matrix[0].hash_1 = hash_1;
    new_control_matrix[0].hash_2 = hash_2;

    if (FuzzUnit(argv, filemap, len, NULL, new_control_matrix))
        return 1;

    if (is_control_matrix_triggered(new_control_matrix, travel_matrix) == false)
    { // the control element is unreachable
        unreachable_control_matrix.insert(hash_total);
    }

    concurrency_test_time++;
    print_result("ConcurrencyFuzzing");
    cout << "-----------------------------------" << endl;
    cout << "concurrency_test_time: " << concurrency_test_time << endl;
    cout << "travel_matrix: " << num_of_matrix_elemet(travel_matrix, TRAVEL_MATRIX_SIZE) << endl;
    cout << "total_travel: " << total_travel.size() << endl;
    cout << "unreachable_control_matrix: " << unreachable_control_matrix.size() << endl;
    cout << "-----------------------------------" << endl;
    return 0;
}

u8 mutate_based_on_adjacency(char **argv, u8 *filemap, u64 len, matrix_element *matrix, int matrix_len,
                             matrix_element *tmp_am)
{
    for (int i = 0; i < matrix_len; i++)
    {
        unsigned long long cm_hash1 = matrix[i].hash_1;
        unsigned long long cm_hash2 = matrix[i].hash_2;

        if (cm_hash1 == 0 && cm_hash2 == 0)
            break;

        for (int j = 0; j < ADJACENCY_MATRIX_SIZE; j++)
        {
            unsigned long long am_hash1 = tmp_am[j].hash_1;
            unsigned long long am_hash2 = tmp_am[j].hash_2;

            if (am_hash1 == 0 && am_hash2 == 0)
                break;

            if (cm_hash1 == am_hash1 && one_element_cm_test(argv, filemap, len, cm_hash2, am_hash2))
                return 1;
            if (cm_hash1 == am_hash2 && one_element_cm_test(argv, filemap, len, cm_hash2, am_hash1))
                return 1;
            if (cm_hash2 == am_hash1 && one_element_cm_test(argv, filemap, len, cm_hash1, am_hash2))
                return 1;
            if (cm_hash2 == am_hash2 && one_element_cm_test(argv, filemap, len, cm_hash1, am_hash1))
                return 1;
        }
    }
    return 0;
}

u8 control_matrix_mutate(char **argv, u64 len, u8 *filemap, matrix_element *tmp_control_matrix)
{
    if (FuzzUnit(argv, filemap, len, NULL, tmp_control_matrix))
        return 1;
    print_result("ConcurrencyFuzzing");
    memcpy(tmp_adjacency_matrix, adjacency_matrix, ADJACENCY_MATRIX_SIZE * sizeof(matrix_element));
    memcpy(tmp_travel_matrix, travel_matrix, TRAVEL_MATRIX_SIZE * sizeof(matrix_element));

    memset(enable_control_matrix, 0, CONTROL_MATRIX_SIZE * sizeof(matrix_element));
    int enable_num = 0;

    for (int i = 0; i < CONTROL_MATRIX_SIZE; i++)
    {
        if (tmp_travel_matrix[i].hash_1 == 0 && tmp_control_matrix[i].hash_2 == 0)
            break;

        for (int j = 0; j < TRAVEL_MATRIX_SIZE; j++)
        {
            if (tmp_travel_matrix[j].hash_1 == 0 && tmp_travel_matrix[j].hash_2 == 0)
                break;

            if (compare_matrix_element(tmp_control_matrix[i], tmp_travel_matrix[j]))
            {
                enable_control_matrix[enable_num] = tmp_control_matrix[i];
                enable_num++;
            }
        }
    }

    memset(new_control_matrix, 0, CONTROL_MATRIX_SIZE * sizeof(matrix_element));
    int new_cm_num = 0;

    cout << "START: concurrency_test_time: " << concurrency_test_time << endl;
    cout << "START: total_travel: " << total_travel.size() << endl;
    cout << "START: unreachable_control_matrix: " << unreachable_control_matrix.size() << endl;

    // if has enable control matrix
    if (enable_num > 0)
    {
        if (mutate_based_on_adjacency(argv, filemap, len, enable_control_matrix, enable_num, tmp_adjacency_matrix))
            return 1;
    }

    // if dont has enable control matrix, just choose from tmp_travel_matrix
    if (mutate_based_on_adjacency(argv, filemap, len, tmp_travel_matrix, TRAVEL_MATRIX_SIZE, tmp_adjacency_matrix))
        return 1;

    cout << "END : concurrency_test_time: " << concurrency_test_time << endl;
    cout << "END : total_travel: " << total_travel.size() << endl;
    cout << "END : unreachable_control_matrix: " << unreachable_control_matrix.size() << endl;

    return 0;
}

#endif