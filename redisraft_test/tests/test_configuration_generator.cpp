#include <redisraft_configuration_generator.hpp>

void test_configuration_generator()
{
    RedisraftConfigurationGenerator ncg;
    assert(ncg.get_configure_string(0, 3) ==
           "/home/zyh/redis/src/redis-server --bind 127.0.1.1 --port 5001 --dbfilename raft0.rdb --loadmodule "
           "/home/zyh/redisraft/redisraft.so raft-log-filename raftlog0.db addr 127.0.1.1:5001");
    assert(ncg.get_configure_string(1, 3) ==
           "/home/zyh/redis/src/redis-server --bind 127.0.1.1 --port 5002 --dbfilename raft1.rdb --loadmodule "
           "/home/zyh/redisraft/redisraft.so raft-log-filename raftlog1.db addr 127.0.1.1:5002");
}

void test_client_configuration_generator()
{
    RedisraftClientConfigurationGenerator nccg;
    assert(nccg.get_configure_string(OP_READ, 3) ==
           "timeout 2 bash /home/zyh/distributed-system-test/redisraft_test/bin/run_client.sh get");
    assert(nccg.get_configure_string(OP_WRITE, 3, 0) ==
           "timeout 2 bash /home/zyh/distributed-system-test/redisraft_test/bin/run_client.sh set 0");
}

int main()
{
    test_configuration_generator();
    test_client_configuration_generator();
}