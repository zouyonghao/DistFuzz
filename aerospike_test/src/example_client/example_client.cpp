#include <aerospike_client.hpp>

int main()
{
    AerospikeClient client(OP_WRITE);
    client.node_count = 5;
    if (client._do())
        return 0;
    return -1;
}