#include <dst_random.h>

#include "../src/operators/aerospike_client.cpp"

uint32_t __dst_get_random_uint32()
{
    static int index = 0;
    static int random_arrays[] = {0, 1, 2, 3, 4};
    return random_arrays[index++];
}

int main(int argc, char const *argv[])
{
    AerospikeClient c(OP_READ);
    c.node_count = 5;
    AerospikeClient c_w(OP_WRITE);
    c_w.node_count = 5;

    c_w._do();

    c._do();
    c_w._do();
    c._do();
    c_w._do();
    c._do();

    c_w._do();
    c_w._do();
    return 0;
}
