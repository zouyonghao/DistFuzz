./stop.sh
./env_clear_tmpfs.sh
rm -rf checkpoint_folder0
rm -rf checkpoint_folder1
rm -rf checkpoint_folder2
rm -rf run_backup
rm -rf run

mkdir checkpoint_folder0
mkdir checkpoint_folder1
mkdir checkpoint_folder2

./run_fuzz_server_normal.sh

cp -r run run_backup

sudo /home/zyh/criu/scripts/criu-ns dump -t $(ps -ef | grep clickhouse-server | grep -v grep | grep config0 | awk '{print $2}') -D checkpoint_folder0 --tcp-established --file-locks -vvv -o dump.log && echo OK

sudo /home/zyh/criu/scripts/criu-ns dump -t $(ps -ef | grep clickhouse-server | grep -v grep | grep config1 | awk '{print $2}') -D checkpoint_folder1 --tcp-established --file-locks -vvv -o dump.log && echo OK

sudo /home/zyh/criu/scripts/criu-ns dump -t $(ps -ef | grep clickhouse-server | grep -v grep | grep config2 | awk '{print $2}') -D checkpoint_folder2 --tcp-established --file-locks -vvv -o dump.log && echo OK

./stop.sh