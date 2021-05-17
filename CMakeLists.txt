cmake_minimum_required(VERSION 3.4.3)
project(distributed_system_test)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(proxy_server)
add_subdirectory(event_control)
add_subdirectory(core)
add_subdirectory(fuzz)

add_subdirectory(braft_test)
add_subdirectory(rethinkdb_test)
add_subdirectory(c-raft_test)
add_subdirectory(nuraft_test)
add_subdirectory(redisraft_test)
add_subdirectory(raftis_test)