#include <braft_test_common.hpp>

void test_configuration_generator()
{
    BraftConfigurationGenerator bcg;
    assert(
        bcg.get_configure_string(0, 3) ==
        "/home/zyh/braft/example/atomic/atomic_server -reuse_addr -ip=127.0.1.1 -reuse_port -election_timeout_ms=100 "
        "-port=8300 -conf=127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0, -data_path=./data0");

    assert(
        bcg.get_configure_string(1, 5) ==
        "/home/zyh/braft/example/atomic/atomic_server -reuse_addr -ip=127.0.1.1 -reuse_port -election_timeout_ms=100 "
        "-port=8301 -conf=127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,127.0.1.1:8303:0,127.0.1.1:8304:0, "
        "-data_path=./data1");
}

int main() { test_configuration_generator(); }