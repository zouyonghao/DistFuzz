# ./run_fuzz_server_normal.sh

# ./run_fuzz_server_with_checkpoint.sh

# ./run_fuzz_server_with_checkpoint_tmpfs.sh

./start_event_control.sh
./start_node0.sh
./start_node1.sh
./start_node2.sh

for (( ; ; )) do /usr/share/zookeeper/bin/zkCli.sh -server 127.0.1.1:9181,127.0.1.1:9182,127.0.1.1:9183  2>/dev/null create /a null && break; done