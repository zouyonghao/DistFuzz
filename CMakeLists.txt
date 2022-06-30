cmake_minimum_required(VERSION 3.4.3)
project(distributed_system_test)

set(CMAKE_CXX_STANDARD 14)

include(CTest)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

# add_subdirectory(proxy_server)
add_subdirectory(event_control)
add_subdirectory(core)
add_subdirectory(fuzz)

add_subdirectory(braft_test)
add_subdirectory(c-raft_test)
add_subdirectory(nuraft_test)
add_subdirectory(redisraft_test)
add_subdirectory(raftis_test)

add_subdirectory(rethinkdb_test)
add_subdirectory(aerospike_test)
add_subdirectory(clickhouse_test)
add_subdirectory(test)

# add_subdirectory(preload_module)

add_subdirectory(zookeeper_test)

add_subdirectory(etcd_test)
add_subdirectory(etcd_test-3517)
add_subdirectory(general_test)