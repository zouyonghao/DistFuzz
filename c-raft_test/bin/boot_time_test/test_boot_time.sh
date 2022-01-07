./stop.sh
./env_clear_tmpfs.sh
rm test_boot_time_result

echo normal >> test_boot_time_result
date +%H:%M:%S.%N >> test_boot_time_result

./run_fuzz_server_normal.sh
./ensure_nodes.sh

date +%H:%M:%S.%N >> test_boot_time_result

./stop.sh
./env_clear_tmpfs.sh

./env_init_checkpoint.sh

echo checkpoint >> test_boot_time_result
date +%H:%M:%S.%N >> test_boot_time_result

./restore_checkpoint.sh
./ensure_nodes_2.sh

date +%H:%M:%S.%N >> test_boot_time_result

./stop.sh
./env_clear_tmpfs.sh

./env_init_checkpoint_tmpfs.sh

echo checkpoint+tmpfs >> test_boot_time_result
date +%H:%M:%S.%N >> test_boot_time_result

./restore_checkpoint_tmpfs.sh
./ensure_nodes_2.sh

date +%H:%M:%S.%N >> test_boot_time_result

./stop.sh
./env_clear_tmpfs.sh