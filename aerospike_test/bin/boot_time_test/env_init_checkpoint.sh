rm checkpoint_folder -rf
mkdir checkpoint_folder

./run_fuzz_server_normal.sh
./ensure_nodes.sh

# cp -r data data_backup

for i in 0 1 2 3 4; do
    mkdir checkpoint_folder/$i
    PID=$(ps -ef | grep asd | grep -v grep | grep aerospike_conf$i | awk '{print $2}')
    echo $PID
    sudo /home/zyh/criu/criu/criu \
        dump --tree $(ps -ef | grep asd | grep -v grep | grep aerospike_conf$i | awk '{print $2}') \
        -D checkpoint_folder/$i -v4 --tcp-established --file-locks --shell-job --ext-unix-sk --ghost 500M -vvv -o dump.log && echo OK
done

cp -r data data_backup

sudo chmod 777 -R checkpoint_folder

./stop.sh
