./stop.sh
./env_clear_tmpfs.sh
rm test_boot_time_result

echo normal >> test_boot_time_result
date +%H:%M:%S.%N >> test_boot_time_result

./run_fuzz_server_normal.sh

date +%H:%M:%S.%N >> test_boot_time_result

./stop.sh
./env_clear_tmpfs.sh

echo checkpoint >> test_boot_time_result
date +%H:%M:%S.%N >> test_boot_time_result

./run_fuzz_server_with_checkpoint.sh

date +%H:%M:%S.%N >> test_boot_time_result

./stop.sh
./env_clear_tmpfs.sh
./env_init_tmpfs.sh

echo checkpoint+tmpfs >> test_boot_time_result
date +%H:%M:%S.%N >> test_boot_time_result

./run_fuzz_server_with_checkpoint_tmpfs.sh

date +%H:%M:%S.%N >> test_boot_time_result

./stop.sh
./env_clear_tmpfs.sh