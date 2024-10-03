#include <rethinkdb_configuration_generator.hpp>

void test_configuration_generator()
{
    RethinkdbConfigurationGenerator rcg;
    assert(rcg.get_configure_string(0, 3) ==
           "/home/zyh/rethinkdb/build/release_clang_system/rethinkdb --bind 127.0.1.1 --cluster-port 5000 "
           "--driver-port 4000 --http-port 8080 -d ./rethinkdb_data0 -j 127.0.1.1:5001 -j 127.0.1.1:5002 ");
    assert(rcg.get_configure_string(1, 3) ==
           "/home/zyh/rethinkdb/build/release_clang_system/rethinkdb --bind 127.0.1.1 --cluster-port 5001 "
           "--driver-port 4001 --http-port 8081 -d ./rethinkdb_data1 -j 127.0.1.1:5000 -j 127.0.1.1:5002 ");
}

void test_client_configuration_generator()
{
    RethinkdbClientConfigurationGenerator rccg;
    assert(rccg.get_configure_string(OP_READ, 3) ==
           "timeout 3 /usr/bin/java -jar "
           "/home/zyh/DistFuzz/rethinkdb_test/client/target/"
           "rethinkdb-test-client-1.0-SNAPSHOT-jar-with-dependencies.jar 3 get");

    assert(rccg.get_configure_string(OP_WRITE, 3, 1) ==
           "timeout 3 /usr/bin/java -jar "
           "/home/zyh/DistFuzz/rethinkdb_test/client/target/"
           "rethinkdb-test-client-1.0-SNAPSHOT-jar-with-dependencies.jar 3 set 1");

    assert(rccg.get_configure_string(OP_CAS, 3, 1, 2) ==
           "timeout 3 /usr/bin/java -jar "
           "/home/zyh/DistFuzz/rethinkdb_test/client/target/"
           "rethinkdb-test-client-1.0-SNAPSHOT-jar-with-dependencies.jar 3 cas 1 2");
}

int main()
{
    test_configuration_generator();
    test_client_configuration_generator();
}