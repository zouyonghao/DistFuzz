./stop.sh
./env_clear_tmpfs.sh

rm checkpoint_folder -rf
mkdir checkpoint_folder

./run_fuzz_server_normal.sh
./ensure_nodes.sh

# cp -r data data_backup

for i in 0 1 2 3 4; do
    mkdir checkpoint_folder/$i
    PID=$(ps -ef | grep redis-server | grep -v grep | grep $(python -c "print 5001+$i") | awk '{print $2}')
    echo PID is $PID
    sudo criu-ns \
        dump --tree $(ps -ef | grep redis-server | grep -v grep | grep $(python -c "print 5001+$i") | awk '{print $2}') \
        -D checkpoint_folder/$i -v4 --shell-job --tcp-established --file-locks -vvv -o dump.log && echo OK
done

cp -r data data_backup

sudo chmod 777 -R checkpoint_folder

./stop.sh
